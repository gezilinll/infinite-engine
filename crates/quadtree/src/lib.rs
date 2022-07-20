extern crate euclid;
extern crate fnv;

use euclid::{Point2D, Size2D};
use fnv::FnvHasher;
use std::cmp::Ord;
use std::collections::HashMap;
use std::hash::BuildHasherDefault;

pub type ItemId = u32;
type FnvHashMap<K, V> = HashMap<K, V, BuildHasherDefault<FnvHasher>>;
type Rect<S> = euclid::Rect<f32, S>;
type Point<S> = Point2D<f32, S>;

pub trait Spatial<S> {
    fn aabb(&self) -> Rect<S>;
}

pub struct QuadTreeConfig {
    max_children: usize,
    min_children: usize,
    max_depth: usize,
}

enum QuadNode<S> {
    Branch {
        aabb: Rect<S>,
        depth: usize,
        element_count: usize,
        elements: Vec<(ItemId, Rect<S>)>,
        children: [(Rect<S>, Box<QuadNode<S>>); 4],
        joint_quad: [Rect<S>; 4],
    },
    Leaf {
        aabb: Rect<S>,
        depth: usize,
        elements: Vec<(ItemId, Rect<S>)>,
    },
}

pub struct QuadTree<T, S> {
    id: u32,
    root: QuadNode<S>,
    config: QuadTreeConfig,
    elements: FnvHashMap<ItemId, (T, Rect<S>)>,
}

impl<T, S> QuadTree<T, S> {
    pub fn new(size: Rect<S>, config: QuadTreeConfig) -> QuadTree<T, S> {
        QuadTree {
            id: 0,
            root: QuadNode::Leaf {
                aabb: size,
                depth: 0,
                elements: Vec::with_capacity(config.max_children),
            },
            elements: HashMap::with_capacity_and_hasher(
                config.max_children * 16, //todo: 有什么深意？
                Default::default(),
            ),
            config,
        }
    }

    pub fn insert_with_box(&mut self, t: T, aabb: Rect<S>) -> Option<ItemId> {
        self.id += 1;
        if self.root.insert(self.id, aabb, &self.config) {
            self.elements.insert(self.id, (t, aabb));
            Some(self.id)
        } else {
            None
        }
    }

    pub fn insert(&mut self, t: T) -> Option<ItemId>
    where
        T: Spatial<S>,
    {
        let aabb = t.aabb();
        self.insert_with_box(t, aabb)
    }

    pub fn query(&self, bounding_box: Rect<S>) -> Vec<(&T, &Rect<S>, ItemId)> {
        let mut ids = vec![];
        self.root.query(&bounding_box, &mut ids);
        ids.sort_by_key(|&(id, _)| id);
        ids.dedup();
        ids.iter()
            .map(|&(id, _)| {
                let &(ref t, ref rect) = match self.elements.get(&id) {
                    Some(e) => e,
                    None => {
                        panic!("looked for {:?}", id);
                    }
                };
                (t, rect, id)
            })
            .collect()
    }

    pub fn remove(&mut self, item_id: ItemId) -> Option<(T, Rect<S>)> {
        match self.elements.remove(&item_id) {
            Some((item, aabb)) => {
                self.root.remove(item_id, aabb, &self.config);
                Some((item, aabb))
            }
            None => None,
        }
    }
}

impl<S> QuadNode<S> {
    fn new_leaf(aabb: Rect<S>, depth: usize, config: &QuadTreeConfig) -> QuadNode<S> {
        QuadNode::Leaf {
            aabb,
            elements: Vec::with_capacity(config.max_children / 2),
            depth,
        }
    }

    fn insert(&mut self, item_id: ItemId, item_aabb: Rect<S>, config: &QuadTreeConfig) -> bool {
        let mut into = None;
        let mut did_insert = false;
        match self {
            &mut QuadNode::Branch {
                ref aabb,
                ref mut element_count,
                ref mut elements,
                ref mut children,
                ref joint_quad,
                ..
            } => {
                if item_aabb.contains(midpoint(aabb))
                    || intersects(&item_aabb, &joint_quad[0])
                    || intersects(&item_aabb, &joint_quad[1])
                    || intersects(&item_aabb, &joint_quad[2])
                    || intersects(&item_aabb, &joint_quad[3])
                {
                    elements.push((item_id, item_aabb));
                    did_insert = true;
                } else {
                    for (child_aabb, child) in children {
                        if intersects(child_aabb, &item_aabb) {
                            if child.insert(item_id, item_aabb, config) {
                                did_insert = true;
                            }
                            break;
                        }
                    }
                }
                if did_insert {
                    *element_count += 1;
                }
            }
            &mut QuadNode::Leaf {
                aabb,
                depth,
                ref mut elements,
            } => {
                if elements.len() > config.max_children && depth != config.max_depth {
                    let mut extracted_children: Vec<(ItemId, Rect<S>)> = Vec::new();
                    std::mem::swap(&mut extracted_children, elements);
                    extracted_children.push((item_id, item_aabb));
                    did_insert = true;

                    let split = split_quad(&aabb);
                    let joint = joint_quad(&split);
                    into = Some((
                        extracted_children,
                        QuadNode::Branch {
                            aabb,
                            depth,
                            element_count: 0,
                            elements: vec![],
                            children: [
                                (
                                    split[0],
                                    Box::new(QuadNode::new_leaf(split[0], depth + 1, config)),
                                ),
                                (
                                    split[1],
                                    Box::new(QuadNode::new_leaf(split[1], depth + 1, config)),
                                ),
                                (
                                    split[2],
                                    Box::new(QuadNode::new_leaf(split[2], depth + 1, config)),
                                ),
                                (
                                    split[3],
                                    Box::new(QuadNode::new_leaf(split[3], depth + 1, config)),
                                ),
                            ],
                            joint_quad: joint,
                        },
                    ));
                } else {
                    elements.push((item_id, item_aabb));
                    did_insert = true;
                }
            }
        }

        if let Some((extracted_children, new_node)) = into {
            *self = new_node;
            for (child_id, child_aabb) in extracted_children {
                self.insert(child_id, child_aabb, config);
            }
        }

        did_insert
    }

    fn remove(&mut self, item_id: ItemId, item_aabb: Rect<S>, config: &QuadTreeConfig) -> bool {
        fn remove_from<S>(v: &mut Vec<(ItemId, Rect<S>)>, item: ItemId) -> bool {
            if let Some(index) = v.iter().position(|a| a.0 == item) {
                v.swap_remove(index);
                true
            } else {
                false
            }
        }

        let mut compact = None;
        let removed = match self {
            &mut QuadNode::Branch {
                ref aabb,
                depth,
                ref mut element_count,
                ref mut elements,
                ref mut children,
                ref joint_quad,
            } => {
                let mut did_remove = false;

                if item_aabb.contains(midpoint(aabb))
                    || intersects(&item_aabb, &joint_quad[0])
                    || intersects(&item_aabb, &joint_quad[1])
                    || intersects(&item_aabb, &joint_quad[2])
                    || intersects(&item_aabb, &joint_quad[3])
                {
                    did_remove = remove_from(elements, item_id);
                } else {
                    for (child_aabb, child_tree) in children {
                        if intersects(child_aabb, &item_aabb) {
                            if child_tree.remove(item_id, item_aabb, config) {
                                did_remove = true;
                            }
                            break;
                        }
                    }
                }

                if did_remove {
                    *element_count -= 1;
                    if *element_count < config.min_children {
                        compact = Some((*element_count, aabb, depth));
                    }
                }

                did_remove
            }
            &mut QuadNode::Leaf {
                aabb: _,
                depth: _,
                ref mut elements,
            } => remove_from(elements, item_id),
        };

        if let Some((size, aabb, depth)) = compact {
            let mut elements: Vec<(ItemId, Rect<S>)> = Vec::with_capacity(size);
            self.query(aabb, &mut elements);
            elements.sort_by(|(id1, _), (id2, _)| id1.cmp(id2));
            elements.dedup();
            *self = QuadNode::Leaf {
                aabb: aabb.clone(),
                depth,
                elements,
            }
        }

        removed
    }

    fn query(&self, query_aabb: &Rect<S>, out: &mut Vec<(ItemId, Rect<S>)>) {
        fn match_all<S>(
            elements: &Vec<(ItemId, Rect<S>)>,
            query_aabb: &Rect<S>,
            out: &mut Vec<(ItemId, Rect<S>)>,
        ) {
            for &(ref child_id, child_aabb) in elements {
                if intersects(query_aabb, &child_aabb) {
                    out.push((*child_id, child_aabb))
                }
            }
        }

        match self {
            &QuadNode::Branch {
                ref elements,
                ref children,
                ..
            } => {
                match_all(elements, query_aabb, out);

                for (child_aabb, child_tree) in children {
                    if intersects(query_aabb, child_aabb) {
                        child_tree.query(query_aabb, out);
                    }
                }
            }
            &QuadNode::Leaf { ref elements, .. } => match_all(elements, query_aabb, out),
        }
    }
}

fn intersects<S>(a: &Rect<S>, b: &Rect<S>) -> bool {
    a.intersects(b)
        || a.min_x() == b.min_x()
        || a.min_y() == b.min_y()
        || a.max_x() == b.max_x()
        || a.max_y() == b.max_y()
}

fn midpoint<S>(rect: &Rect<S>) -> Point<S> {
    let origin = rect.origin;
    let half = rect.size.to_vector() / 2.0;
    origin + half
}

fn split_quad<S>(rect: &Rect<S>) -> [Rect<S>; 4] {
    use euclid::vec2;
    let origin = rect.origin;
    let half = rect.size / 2.0;
    [
        Rect::new(origin, half),                                 // 左上
        Rect::new(origin + vec2(half.width, 0.0), half),         // 右上
        Rect::new(origin + vec2(0.0, half.height), half),        // 左下
        Rect::new(origin + vec2(half.width, half.height), half), // 右下
    ]
}

fn joint_quad<S>(quad: &[Rect<S>; 4]) -> [Rect<S>; 4] {
    [
        Rect::new(
            quad[0].origin,
            Size2D::new(quad[0].width() + quad[1].width(), quad[0].height()),
        ), // 左上 + 右上
        Rect::new(
            quad[0].origin,
            Size2D::new(quad[0].width(), quad[0].height() + quad[2].height()),
        ), // 左上 + 左下
        Rect::new(
            quad[2].origin,
            Size2D::new(quad[2].width() + quad[3].width(), quad[2].height()),
        ), // 左下 + 右下
        Rect::new(
            quad[1].origin,
            Size2D::new(quad[1].width(), quad[1].height() + quad[3].height()),
        ), // 右上 + 右下
    ]
}

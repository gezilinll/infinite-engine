extern crate fnv;

pub mod rect;

use fnv::FnvHasher;
use rect::Rect;
use std::collections::HashMap;
use std::hash::BuildHasherDefault;

pub type ItemId = u32;
type FnvHashMap<K, V> = HashMap<K, V, BuildHasherDefault<FnvHasher>>;

pub trait Spatial {
    fn aabb(&self) -> Rect;
}

pub struct QuadTreeConfig {
    pub max_children: usize,
    pub min_children: usize,
    pub max_depth: usize,
}

enum QuadNode {
    Branch {
        aabb: Rect,
        depth: usize,
        element_count: usize,
        elements: Vec<(ItemId, Rect)>,
        children: [(Rect, Box<QuadNode>); 4],
        joint_quad: [Rect; 4],
    },
    Leaf {
        aabb: Rect,
        depth: usize,
        elements: Vec<(ItemId, Rect)>,
    },
}

pub struct QuadTree<T> {
    id: u32,
    root: QuadNode,
    config: QuadTreeConfig,
    elements: FnvHashMap<ItemId, (T, Rect)>,
}

impl<T> QuadTree<T> {
    pub fn new(size: Rect, config: QuadTreeConfig) -> QuadTree<T> {
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

    pub fn insert_with_box(&mut self, t: T, aabb: Rect) -> Option<ItemId> {
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
        T: Spatial,
    {
        let aabb = t.aabb();
        self.insert_with_box(t, aabb)
    }

    pub fn query(&self, bounding_box: Rect) -> Vec<(&T, &Rect, ItemId)> {
        let mut ids = vec![];
        self.root.query(&bounding_box, &mut ids);
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

    pub fn remove(&mut self, item_id: ItemId) -> Option<(T, Rect)> {
        match self.elements.remove(&item_id) {
            Some((item, aabb)) => {
                self.root.remove(item_id, aabb, &self.config);
                Some((item, aabb))
            }
            None => None,
        }
    }
}

impl QuadNode {
    fn new_leaf(aabb: Rect, depth: usize, config: &QuadTreeConfig) -> QuadNode {
        QuadNode::Leaf {
            aabb,
            elements: Vec::with_capacity(config.max_children / 2),
            depth,
        }
    }

    fn insert(&mut self, item_id: ItemId, item_aabb: Rect, config: &QuadTreeConfig) -> bool {
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
                if item_aabb.contains(aabb.midpoint())
                    || item_aabb.intersects(&joint_quad[0])
                    || item_aabb.intersects(&joint_quad[1])
                    || item_aabb.intersects(&joint_quad[2])
                    || item_aabb.intersects(&joint_quad[3])
                {
                    elements.push((item_id, item_aabb));
                    did_insert = true;
                } else {
                    for (child_aabb, child) in children {
                        if child_aabb.intersects(&item_aabb) {
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
                    let mut extracted_children: Vec<(ItemId, Rect)> = Vec::new();
                    std::mem::swap(&mut extracted_children, elements);
                    extracted_children.push((item_id, item_aabb));
                    did_insert = true;

                    let split = aabb.split_quad();
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

    fn remove(&mut self, item_id: ItemId, item_aabb: Rect, config: &QuadTreeConfig) -> bool {
        fn remove_from(v: &mut Vec<(ItemId, Rect)>, item: ItemId) -> bool {
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

                if item_aabb.contains(aabb.midpoint())
                    || item_aabb.intersects(&joint_quad[0])
                    || item_aabb.intersects(&joint_quad[1])
                    || item_aabb.intersects(&joint_quad[2])
                    || item_aabb.intersects(&joint_quad[3])
                {
                    did_remove = remove_from(elements, item_id);
                } else {
                    for (child_aabb, child_tree) in children {
                        if child_aabb.intersects(&item_aabb) {
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
            let mut elements: Vec<(ItemId, Rect)> = Vec::with_capacity(size);
            self.query(aabb, &mut elements);
            *self = QuadNode::Leaf {
                aabb: aabb.clone(),
                depth,
                elements,
            }
        }

        removed
    }

    fn query(&self, query_aabb: &Rect, out: &mut Vec<(ItemId, Rect)>) {
        fn match_all(
            elements: &Vec<(ItemId, Rect)>,
            query_aabb: &Rect,
            out: &mut Vec<(ItemId, Rect)>,
        ) {
            for &(ref child_id, child_aabb) in elements {
                if query_aabb.intersects(&child_aabb) {
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
                    if query_aabb.intersects(child_aabb) {
                        child_tree.query(query_aabb, out);
                    }
                }
            }
            &QuadNode::Leaf { ref elements, .. } => match_all(elements, query_aabb, out),
        }
    }
}

fn joint_quad(quad: &[Rect; 4]) -> [Rect; 4] {
    [
        quad[0].joint(quad[1]), // 左上 + 右上
        quad[0].joint(quad[2]), // 左上 + 左下
        quad[2].joint(quad[3]), // 左下 + 右下
        quad[1].joint(quad[3]), // 右上 + 右下
    ]
}

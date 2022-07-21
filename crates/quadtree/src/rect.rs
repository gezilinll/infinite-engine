#[derive(Clone, Copy)]
pub struct Rect {
    pub left: f32,
    pub right: f32,
    pub top: f32,
    pub bottom: f32,
}

impl Rect {
    pub fn new(left: f32, right: f32, top: f32, bottom: f32) -> Rect {
        Rect {
            left,
            right,
            top,
            bottom,
        }
    }

    pub fn intersects(&self, other: &Rect) -> bool {
        let result = self.left < other.right
            && self.right > other.left
            && self.bottom < other.top
            && self.top > other.bottom;
        result
            || self.left == other.left
            || self.right == other.right
            || self.top == other.top
            || self.bottom == other.bottom
    }

    pub fn midpoint(&self) -> (f32, f32) {
        (
            (self.right + self.left) / 2.0,
            (self.top + self.bottom / 2.0),
        )
    }

    pub fn split_quad(&self) -> [Rect; 4] {
        let half = (
            (self.right - self.left) / 2.0,
            (self.top - self.bottom) / 2.0,
        );
        [
            Rect::new(self.left, self.left + half.0, self.top, self.top - half.1), // 左上
            Rect::new(self.left + half.0, self.top, self.right, self.top - half.1), // 右上
            Rect::new(
                self.left,
                self.left + half.0,
                self.top - half.1,
                self.bottom,
            ), // 左下
            Rect::new(
                self.left + half.0,
                self.right,
                self.top - half.1,
                self.bottom,
            ), // 右下
        ]
    }

    pub fn joint(&self, other: Rect) -> Rect {
        Rect {
            left: self.left.min(other.left),
            right: self.right.max(other.right),
            top: self.top.max(other.top),
            bottom: self.bottom.min(other.bottom),
        }
    }

    pub fn contains(&self, point: (f32, f32)) -> bool {
        self.left <= point.0
            && self.right >= point.0
            && self.top >= point.1
            && self.bottom <= point.1
    }
}

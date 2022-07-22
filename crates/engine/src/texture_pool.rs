use std::collections::HashMap;

use image::DynamicImage;

use crate::{
    texture::{self, from_image, Texture},
    wgpu_holder::WGPUHolder,
};

pub struct TexturePool {
    cached: HashMap<String, Texture>,
}

impl TexturePool {
    pub fn new() -> Self {
        Self {
            cached: HashMap::new(),
        }
    }

    pub fn get(&mut self, key: &String, image: &DynamicImage, holder: &mut WGPUHolder) -> &Texture {
        if self.cached.contains_key(key) {
            self.cached.get(key).as_ref().unwrap()
        } else {
            let texture = from_image(holder, image, None);
            self.cached.insert(key.clone(), texture);
            self.cached.get(key).as_ref().unwrap()
        }
    }
}

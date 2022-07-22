use anyhow::{Context, Ok, Result};
use image::{GenericImageView, GrayImage, Pixel};
use std::{
    collections::HashMap,
    sync::{Arc, Mutex},
};

pub struct ImagePool {
    cached: HashMap<String, Arc<Mutex<image::DynamicImage>>>,
}

impl ImagePool {
    pub fn new() -> Self {
        Self {
            cached: HashMap::new(),
        }
    }

    pub fn get(&mut self, file_path: &String) -> Result<Arc<Mutex<image::DynamicImage>>> {
        if self.cached.contains_key(file_path) {
            Ok(Arc::clone(self.cached.get(file_path).unwrap()))
        } else {
            let mut image = image::open(file_path.clone())?;
            let result = Arc::new(Mutex::new(image));
            self.cached.insert(file_path.clone(), Arc::clone(&result));
            Ok(Arc::clone(&result))
        }
    }
}

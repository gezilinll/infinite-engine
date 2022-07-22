use std::{collections::HashMap, fs::File, io::Write};

use element::ImageElement;
use quadtree::{rect::Rect, QuadTree, QuadTreeConfig};
use render_quadtree::QuadTreeRenderer;
use wgpu_holder::WGPUHolder;
use winit::window::Window;

pub mod element;
mod render_quadtree;
mod texture;
mod vertex;
mod wgpu_holder;

pub struct Canvas {
    elements: HashMap<u32, ImageElement>,
    area: Rect,
    holder: WGPUHolder,
}

impl Canvas {
    pub async fn new(window: &Window, area: Rect) -> Canvas {
        let size = window.inner_size();
        let instance = wgpu::Instance::new(wgpu::Backends::all());
        let surface = unsafe { instance.create_surface(window) };
        let adapter = instance
            .request_adapter(&wgpu::RequestAdapterOptions {
                power_preference: wgpu::PowerPreference::default(),
                compatible_surface: Some(&surface),
                force_fallback_adapter: false,
            })
            .await
            .unwrap();
        let (device, queue) = adapter
            .request_device(
                &wgpu::DeviceDescriptor {
                    label: None,
                    features: wgpu::Features::POLYGON_MODE_LINE,
                    // WebGL doesn't support all of wgpu's features, so if
                    // we're building for the web we'll have to disable some.
                    limits: if cfg!(target_arch = "wasm32") {
                        wgpu::Limits::downlevel_webgl2_defaults()
                    } else {
                        wgpu::Limits::default()
                    },
                },
                None, // Trace path
            )
            .await
            .unwrap();

        let config = wgpu::SurfaceConfiguration {
            usage: wgpu::TextureUsages::RENDER_ATTACHMENT,
            format: surface.get_preferred_format(&adapter).unwrap(),
            width: size.width,
            height: size.height,
            present_mode: wgpu::PresentMode::Fifo,
        };

        surface.configure(&device, &config);

        Self {
            elements: HashMap::new(),
            area,
            holder: WGPUHolder {
                surface,
                device,
                queue,
                config,
            },
        }
    }

    pub fn add_element(&mut self, element: ImageElement) {
        self.elements.insert(element.get_id(), element);
    }

    pub fn render(&mut self, focus: Rect) {
        let mut quad_tree = QuadTree::<ImageElement>::new(
            self.area,
            QuadTreeConfig {
                max_children: 2,
                min_children: 1,
                max_depth: 8,
            },
        );
        for element in &self.elements {
            quad_tree.insert(element.1.copy_for_quadtree());
        }
        // let mut log_file = File::create("/Users/linbinghe/Projects/Infinite-canvas/log.txt").unwrap();
        // write!(&mut log_file, "{:#?}", quad_tree).unwrap();

        let output = self.holder.surface.get_current_texture().unwrap();
        let view = output
            .texture
            .create_view(&wgpu::TextureViewDescriptor::default());
        let mut elements_to_render = quad_tree.query(focus);
        for (element, _, _) in &mut elements_to_render {
            let element = self.elements.get_mut(&element.get_id()).unwrap();
            element.submit(&focus, &mut self.holder, &view);
        }

        QuadTreeRenderer::render(&quad_tree, &mut self.holder, &focus, &view);

        output.present();
    }
}

mod image_pool;

use std::{
    collections::HashMap,
    env,
    sync::{Arc, Mutex},
    thread,
    time::{Duration, Instant},
};

use engine::{element::ImageElement, Canvas};
use image_pool::ImagePool;
use quadtree::rect::Rect;
use rand::Rng;
use winit::{
    event::{ElementState, Event, KeyboardInput, VirtualKeyCode, WindowEvent},
    event_loop::{ControlFlow, EventLoop},
};

fn test_canvas(canvas: Arc<Mutex<Canvas>>, image_pool: Arc<Mutex<ImagePool>>, image_index: u32) {
    let current_dir = env::current_dir().unwrap().to_str().unwrap().to_string();
    let image_dir = if current_dir.contains("examples") {
        env::current_dir().unwrap().to_str().unwrap().to_string() + "/"
    } else {
        env::current_dir().unwrap().to_str().unwrap().to_string() + "/examples/image/"
    };
    thread::Builder::new()
        .name("add_test_element".to_string())
        .spawn(move || {
            let mut exist_id: HashMap<u32, u32> = HashMap::new();
            exist_id.insert(image_index, 1);
            let item_width = 160;
            let max_elements = 166;
            let mut element_count = 1;
            let mut element_id = 100;
            loop {
                if element_count >= max_elements {
                    break;
                }
                let mut rng = rand::thread_rng();
                let image_center_x: i32 = rng.gen_range(-900..900);
                let image_center_y: i32 = rng.gen_range(-600..600);
                let image_index: u32 = rng.gen_range(1..109);
                if exist_id.contains_key(&image_index) {
                    if element_count < 100 {
                        continue;
                    }
                }
                let image_element;
                {
                    let file_path = image_dir.clone() + &image_index.to_string() + ".jpg";
                    let cached_image = image_pool.lock().unwrap().get(&file_path).unwrap();
                    let image = cached_image.lock().unwrap();
                    let height = item_width as f32 / (image.width() as f32 / image.height() as f32);
                    let image_rect = Rect::new(
                        image_center_x as f32 - item_width as f32 / 2.0,
                        image_center_x as f32 + item_width as f32 / 2.0,
                        image_center_y as f32 + height / 2.0,
                        image_center_y as f32 - height / 2.0,
                    );
                    image_element = ImageElement::new(
                        element_id,
                        file_path,
                        Arc::clone(&cached_image),
                        (image.width(), image.height()),
                        image_rect,
                    );
                }
                {
                    canvas.lock().unwrap().add_element(image_element);
                }
                exist_id.insert(image_index, 1);
                element_id += 1;
                element_count += 1;
                thread::sleep(Duration::from_millis(16));
            }

            thread::sleep(Duration::from_millis(300));

            let mut scale = 1.0 as f32;
            loop {
                if scale >= 6.6 {
                    break;
                }
                scale += 0.1;
                {
                    canvas.lock().unwrap().scale(scale);
                }
                thread::sleep(Duration::from_millis(15));
            }
        })
        .ok();
}

fn main() {
    env_logger::init();
    let event_loop = EventLoop::new();
    let title = env!("CARGO_PKG_NAME");
    let window = winit::window::WindowBuilder::new()
        .with_title(title)
        .build(&event_loop)
        .unwrap();
    let window_size = window.inner_size();
    let focus_rect = Rect::new(
        -(window_size.width as f32 / 2.0),
        window_size.width as f32 / 2.0,
        window_size.height as f32 / 2.0,
        -(window_size.height as f32 / 2.0),
    );

    let canvas = pollster::block_on(Canvas::new(
        &window,
        Rect::new(
            -(window_size.width as f32),
            window_size.width as f32,
            window_size.height as f32,
            -(window_size.height as f32),
        ),
    ));
    let canvas = Arc::new(Mutex::new(canvas));
    let image_pool = Arc::new(Mutex::new(ImagePool::new()));
    let image_index;
    {
        let image_element;
        {
            let current_dir = env::current_dir().unwrap().to_str().unwrap().to_string();
            let image_dir = if current_dir.contains("examples") {
                env::current_dir().unwrap().to_str().unwrap().to_string() + "/"
            } else {
                env::current_dir().unwrap().to_str().unwrap().to_string() + "/examples/image/"
            };
            let item_width = 80;
            let mut rng = rand::thread_rng();
            image_index = rng.gen_range(1..109);
            let file_path = image_dir.clone() + &image_index.to_string() + ".jpg";
            let cached_image = image_pool.lock().unwrap().get(&file_path).unwrap();
            let image = cached_image.lock().unwrap();
            let height = item_width as f32 / (image.width() as f32 / image.height() as f32);
            let image_rect = Rect::new(
                -(item_width as f32) / 2.0,
                item_width as f32 / 2.0,
                height / 2.0,
                -(height / 2.0),
            );
            image_element = ImageElement::new(
                88,
                file_path,
                Arc::clone(&cached_image),
                (image.width(), image.height()),
                image_rect,
            );
        }
        canvas.lock().unwrap().add_element(image_element);
    }
    test_canvas(Arc::clone(&canvas), Arc::clone(&image_pool), image_index);

    event_loop.run(move |event, _, control_flow| {
        *control_flow = ControlFlow::Poll;
        match event {
            Event::MainEventsCleared => window.request_redraw(),
            Event::WindowEvent {
                ref event,
                window_id,
            } if window_id == window.id() => match event {
                WindowEvent::CloseRequested
                | WindowEvent::KeyboardInput {
                    input:
                        KeyboardInput {
                            state: ElementState::Pressed,
                            virtual_keycode: Some(VirtualKeyCode::Escape),
                            ..
                        },
                    ..
                } => *control_flow = ControlFlow::Exit,
                _ => {}
            },
            Event::RedrawRequested(window_id) if window_id == window.id() => {
                canvas.lock().unwrap().render(focus_rect);
            }
            _ => {}
        }
    });
}

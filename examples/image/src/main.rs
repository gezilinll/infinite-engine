use std::{env, time::Instant};

use engine::{element::ImageElement, Canvas};
use quadtree::rect::Rect;
use winit::{
    event::{ElementState, Event, KeyboardInput, VirtualKeyCode, WindowEvent},
    event_loop::{ControlFlow, EventLoop},
};

fn add_test_element(canvas: &mut Canvas, window_w: u32, window_h: u32) {
    let current_dir = env::current_dir().unwrap().to_str().unwrap().to_string();
    let image_path = if current_dir.contains("examples") {
        env::current_dir().unwrap().to_str().unwrap().to_string() + "/1.jpg"
    } else {
        env::current_dir().unwrap().to_str().unwrap().to_string() + "/examples/image/2.jpg"
    };

    let item_width = 50;
    let mut current_index = 0;
    let mut element_id = 100 as u32;
    let origin_rect = Rect::new(0., 50., 0., -50.);
    let image_element = ImageElement::new(element_id, image_path.clone(), origin_rect);
    canvas.add_element(image_element);
    loop {
        if current_index == 25 {
            break;
        }
        element_id += 1;
        let image_element = ImageElement::new(
            element_id,
            image_path.clone(),
            Rect::new(
                origin_rect.left,
                origin_rect.right,
                origin_rect.top + (current_index * item_width) as f32,
                origin_rect.bottom + (current_index * item_width) as f32,
            ),
        );
        canvas.add_element(image_element);
        element_id += 1;
        let image_element = ImageElement::new(
            element_id,
            image_path.clone(),
            Rect::new(
                origin_rect.left,
                origin_rect.right,
                origin_rect.top - (current_index * item_width) as f32,
                origin_rect.bottom - (current_index * item_width) as f32,
            ),
        );
        canvas.add_element(image_element);
        current_index += 1;
    }
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

    let mut canvas = pollster::block_on(Canvas::new(
        &window,
        Rect::new(-5000., 5000., 5000., -5000.),
    ));
    add_test_element(&mut canvas, window_size.width, window_size.height);

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
                // canvas.render(Rect::new(
                //     focus_rect.left - 300.,
                //     focus_rect.right - 300.,
                //     focus_rect.top - 300.,
                //     focus_rect.bottom - 300.,
                // ));
                let current = Instant::now();
                canvas.render(focus_rect);
                println!("render cost:{}", current.elapsed().subsec_millis());
            }
            _ => {}
        }
    });
}

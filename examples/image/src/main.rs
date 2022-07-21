use std::env;

use engine::{element::ImageElement, Canvas};
use quadtree::rect::Rect;
use winit::{
    event::{ElementState, Event, KeyboardInput, VirtualKeyCode, WindowEvent},
    event_loop::{ControlFlow, EventLoop},
};

fn add_test_element() {
    
}

fn main() {
    let current_dir = env::current_dir().unwrap().to_str().unwrap().to_string();
    let image_path = if current_dir.contains("examples") {
        env::current_dir().unwrap().to_str().unwrap().to_string() + "/cpu-gpu-compare.png"
    } else {
        env::current_dir().unwrap().to_str().unwrap().to_string()
            + "/examples/image/cpu-gpu-compare.png"
    };

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
    let image_element = ImageElement::new(
        100,
        image_path,
        Rect::new(
            focus_rect.left,
            focus_rect.left + 1476.,
            focus_rect.top,
            focus_rect.top - 570.,
        ),
    );
    canvas.add_element(image_element);

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
                canvas.render(focus_rect);
            }
            _ => {}
        }
    });
}

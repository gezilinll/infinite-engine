use quadtree::rect::Rect;

#[repr(C)]
#[derive(Copy, Clone, Debug, bytemuck::Pod, bytemuck::Zeroable, Default)]
pub struct Position {
    pub position: [f32; 3],
}

impl Position {
    pub fn desc<'a>() -> wgpu::VertexBufferLayout<'a> {
        use std::mem;
        wgpu::VertexBufferLayout {
            array_stride: mem::size_of::<Position>() as wgpu::BufferAddress,
            step_mode: wgpu::VertexStepMode::Vertex,
            attributes: &[wgpu::VertexAttribute {
                offset: 0,
                shader_location: 0,
                format: wgpu::VertexFormat::Float32x3,
            }],
        }
    }
}

#[repr(C)]
#[derive(Copy, Clone, Debug, bytemuck::Pod, bytemuck::Zeroable, Default)]
pub struct Vertex {
    position: [f32; 3],
    tex_coords: [f32; 2],
}

impl Vertex {
    pub fn desc<'a>() -> wgpu::VertexBufferLayout<'a> {
        use std::mem;
        wgpu::VertexBufferLayout {
            array_stride: mem::size_of::<Vertex>() as wgpu::BufferAddress,
            step_mode: wgpu::VertexStepMode::Vertex,
            attributes: &[
                wgpu::VertexAttribute {
                    offset: 0,
                    shader_location: 0,
                    format: wgpu::VertexFormat::Float32x3,
                },
                wgpu::VertexAttribute {
                    offset: mem::size_of::<[f32; 3]>() as wgpu::BufferAddress,
                    shader_location: 1,
                    format: wgpu::VertexFormat::Float32x2,
                },
            ],
        }
    }

    pub fn get_position(object: &Rect, canvas: &Rect) -> Vec<Position> {
        let canvas_mid = (
            (canvas.left + canvas.right) / 2.0,
            (canvas.top + canvas.bottom) / 2.0,
        );
        let canvas_half = (
            (canvas.right - canvas.left) / 2.0,
            (canvas.top - canvas.bottom) / 2.0,
        );

        let v_left = (object.left - canvas_mid.0) / canvas_half.0;
        let v_right = (object.right - canvas_mid.0) / canvas_half.0;
        let v_top = (object.top - canvas_mid.1) / canvas_half.1;
        let v_bottom = (object.bottom - canvas_mid.1) / canvas_half.1;

        vec![
            Position {
                position: [v_left, v_bottom, 0.0],
            }, // A
            Position {
                position: [v_right, v_bottom, 0.0],
            }, // B
            Position {
                position: [v_left, v_top, 0.0],
            }, // C
            Position {
                position: [v_right, v_top, 0.0],
            },
        ]
    }

    pub fn get_vertex(object: &Rect, canvas: &Rect, img_w: u32, img_h: u32) -> Vec<Vertex> {
        let canvas_mid = (
            (canvas.left + canvas.right) / 2.0,
            (canvas.top + canvas.bottom) / 2.0,
        );
        let canvas_half = (
            (canvas.right - canvas.left) / 2.0,
            (canvas.top - canvas.bottom) / 2.0,
        );

        let v_left = ((object.left - canvas_mid.0) / canvas_half.0).max(-1.0);
        let v_right = ((object.right - canvas_mid.0) / canvas_half.0).min(1.0);
        let v_top = ((object.top - canvas_mid.1) / canvas_half.1).min(1.0);
        let v_bottom = ((object.bottom - canvas_mid.1) / canvas_half.1).max(-1.0);

        let t_left = if object.left < canvas.left {
            (canvas.left - object.left) / (object.right - object.left)
        } else {
            0.0
        };
        let t_right = if object.right > canvas.right {
            1.0 - (object.right - canvas.right) / (object.right - object.left)
        } else {
            1.0
        };
        let t_top = if object.top > canvas.top {
            (object.top - canvas.top) / (object.top - object.bottom)
        } else {
            0.0
        };
        let t_bottom = if object.bottom < canvas.bottom {
            1.0 - (canvas.bottom - object.bottom) / (object.top - object.bottom)
        } else {
            1.0
        };

        vec![
            Vertex {
                position: [v_left, v_bottom, 0.0],
                tex_coords: [t_left, t_bottom],
            }, // A
            Vertex {
                position: [v_right, v_bottom, 0.0],
                tex_coords: [t_right, t_bottom],
            }, // B
            Vertex {
                position: [v_left, v_top, 0.0],
                tex_coords: [t_left, t_top],
            }, // C
            Vertex {
                position: [v_right, v_top, 0.0],
                tex_coords: [t_right, t_top],
            },
        ]
    }
}

pub const INDICES: &[u16] = &[0, 1, 2, 1, 3, 2];

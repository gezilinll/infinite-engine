use quadtree::{rect::Rect, QuadNode, QuadTree};
use wgpu::util::DeviceExt;

use crate::{
    vertex::{Position, Vertex},
    wgpu_holder::WGPUHolder,
};

pub struct QuadTreeRenderer {}

impl QuadTreeRenderer {
    fn render_node(
        node: &QuadNode,
        focus: &Rect,
        holder: &mut WGPUHolder,
        output: &wgpu::TextureView,
    ) -> (Vec<Position>, Vec<u16>) {
        match node {
            &QuadNode::Branch { ref children, .. } => {
                let mut result: (Vec<Position>, Vec<u16>) = (vec![], vec![]);
                for child in children {
                    let mut child_result = Self::render_node(&child.1, focus, holder, output);
                    if result.0.len() > 0 && child_result.0.len() > 0 {
                        let last_vertex_position = result.0[*(result.1.last().unwrap()) as usize].clone();
                        let child_first_position = child_result.0.first().unwrap().clone();
                        if last_vertex_position.position[0] != child_first_position.position[0]
                            && last_vertex_position.position[1] != child_first_position.position[1]
                        {
                            let mut child_indices = vec![];
                            result.0.push(Position {
                                position: [
                                    child_first_position.position[0],
                                    last_vertex_position.position[1],
                                    0.0,
                                ],
                            });
                            child_indices.push((result.0.len() - 1) as u16);
                            for index in 0..child_result.1.len() {
                                child_result.1[index] += result.0.len() as u16;
                            }
                            child_indices.extend_from_slice(child_result.1.as_slice());

                            result.0.extend_from_slice(child_result.0.as_slice());
                            result.1.extend_from_slice(child_indices.as_slice());
                        } else {
                            for index in 0..child_result.1.len() {
                                child_result.1[index] += result.0.len() as u16;
                            }
                            result.0.extend_from_slice(child_result.0.as_slice());
                            result.1.extend_from_slice(child_result.1.as_slice());
                        }
                    } else {
                        result.0.extend_from_slice(child_result.0.as_slice());
                        result.1.extend_from_slice(child_result.1.as_slice());
                    }
                }
                result
            }
            &QuadNode::Leaf { aabb, .. } => {
                if aabb.intersects(&focus) {
                    let vertex = Vertex::get_position(&aabb, focus);
                    let indices: &[u16] = &[0, 1, 3, 2, 0];
                    return (vertex, indices.to_vec());
                }
                (vec![], vec![])
            }
        }
    }

    pub fn render<T>(
        quadtree: &QuadTree<T>,
        holder: &mut WGPUHolder,
        focus: &Rect,
        output: &wgpu::TextureView,
    ) {
        let vertex_indices = Self::render_node(&quadtree.root, focus, holder, output);
        let mut encoder = holder
            .device
            .create_command_encoder(&wgpu::CommandEncoderDescriptor {
                label: Some("Render Encoder"),
            });
        let vertex_buffer = holder
            .device
            .create_buffer_init(&wgpu::util::BufferInitDescriptor {
                label: None,
                contents: bytemuck::cast_slice(vertex_indices.0.as_slice()) as &[u8],
                usage: wgpu::BufferUsages::VERTEX,
            });
        let index_buffer = holder
            .device
            .create_buffer_init(&wgpu::util::BufferInitDescriptor {
                label: None,
                contents: bytemuck::cast_slice(vertex_indices.1.as_slice()) as &[u8],
                usage: wgpu::BufferUsages::INDEX,
            });
        let shader_des = wgpu::ShaderModuleDescriptor {
            label: None,
            source: wgpu::ShaderSource::Wgsl(include_str!("line.wgsl").into()),
        };
        let shader = holder.device.create_shader_module(&shader_des);
        let render_pipeline_layout =
            holder
                .device
                .create_pipeline_layout(&wgpu::PipelineLayoutDescriptor {
                    label: Some("Render Pipeline Layout"),
                    bind_group_layouts: &[],
                    push_constant_ranges: &[],
                });
        let render_pipeline =
            holder
                .device
                .create_render_pipeline(&wgpu::RenderPipelineDescriptor {
                    label: Some("Render Pipeline"),
                    layout: Some(&render_pipeline_layout),
                    vertex: wgpu::VertexState {
                        module: &shader,
                        entry_point: "vs_main",
                        buffers: &vec![Position::desc()],
                    },
                    fragment: Some(wgpu::FragmentState {
                        module: &shader,
                        entry_point: "fs_main",
                        targets: &[wgpu::ColorTargetState {
                            format: holder.config.format,
                            blend: Some(wgpu::BlendState {
                                color: wgpu::BlendComponent::OVER,
                                alpha: wgpu::BlendComponent::OVER,
                            }),
                            write_mask: wgpu::ColorWrites::ALL,
                        }],
                    }),
                    primitive: wgpu::PrimitiveState {
                        topology: wgpu::PrimitiveTopology::LineStrip,
                        strip_index_format: None,
                        front_face: wgpu::FrontFace::Ccw,
                        cull_mode: Some(wgpu::Face::Back),
                        // Setting this to anything other than Fill requires Features::POLYGON_MODE_LINE
                        // or Features::POLYGON_MODE_POINT
                        polygon_mode: wgpu::PolygonMode::Line,
                        // Requires Features::DEPTH_CLIP_CONTROL
                        unclipped_depth: false,
                        // Requires Features::CONSERVATIVE_RASTERIZATION
                        conservative: false,
                    },
                    depth_stencil: None,
                    multisample: wgpu::MultisampleState {
                        count: 1,
                        mask: !0,
                        alpha_to_coverage_enabled: false,
                    },
                    // If the pipeline will be used with a multiview render pass, this
                    // indicates how many array layers the attachments will have.
                    multiview: None,
                });
        {
            let mut render_pass = encoder.begin_render_pass(&wgpu::RenderPassDescriptor {
                label: Some("Render Pass"),
                color_attachments: &[wgpu::RenderPassColorAttachment {
                    view: &output,
                    resolve_target: None,
                    ops: wgpu::Operations {
                        load: wgpu::LoadOp::Load,
                        store: true,
                    },
                }],
                depth_stencil_attachment: None,
            });
            render_pass.set_pipeline(&render_pipeline);
            render_pass.set_vertex_buffer(0, vertex_buffer.slice(..));
            render_pass.set_index_buffer(index_buffer.slice(..), wgpu::IndexFormat::Uint16);
            render_pass.draw_indexed(0..vertex_indices.1.len() as u32, 0, 0..1);
        }

        holder.queue.submit(std::iter::once(encoder.finish()));
    }
}

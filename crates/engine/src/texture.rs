use std::{borrow::Cow, num::NonZeroU32};

use image::GenericImageView;

use crate::wgpu_holder::WGPUHolder;

const TEXTURE_FORMAT: wgpu::TextureFormat = wgpu::TextureFormat::Rgba8UnormSrgb;

pub struct Texture {
    pub texture: wgpu::Texture,
    pub view: Vec<wgpu::TextureView>,
    pub sampler: wgpu::Sampler,
    pub width: u32,
    pub height: u32,
}

pub fn from_image(
    holder: &mut WGPUHolder,
    img: &image::DynamicImage,
    label: Option<&str>,
) -> Texture {
    let rgba = img.to_rgba8();
    let dimensions = img.dimensions();

    let size = wgpu::Extent3d {
        width: dimensions.0,
        height: dimensions.1,
        depth_or_array_layers: 1,
    };
    let texture = holder.device.create_texture(&wgpu::TextureDescriptor {
        label,
        size,
        mip_level_count: 5,
        sample_count: 1,
        dimension: wgpu::TextureDimension::D2,
        format: wgpu::TextureFormat::Rgba8UnormSrgb,
        usage: wgpu::TextureUsages::TEXTURE_BINDING
            | wgpu::TextureUsages::COPY_DST
            | wgpu::TextureUsages::RENDER_ATTACHMENT,
    });
    holder.queue.write_texture(
        wgpu::ImageCopyTexture {
            aspect: wgpu::TextureAspect::All,
            texture: &texture,
            mip_level: 0,
            origin: wgpu::Origin3d::ZERO,
        },
        &rgba,
        wgpu::ImageDataLayout {
            offset: 0,
            bytes_per_row: NonZeroU32::new(4 * dimensions.0),
            rows_per_image: NonZeroU32::new(dimensions.1),
        },
        size,
    );
    let sampler = holder.device.create_sampler(&wgpu::SamplerDescriptor {
        address_mode_u: wgpu::AddressMode::ClampToEdge,
        address_mode_v: wgpu::AddressMode::ClampToEdge,
        address_mode_w: wgpu::AddressMode::ClampToEdge,
        mag_filter: wgpu::FilterMode::Linear,
        min_filter: wgpu::FilterMode::Nearest,
        mipmap_filter: wgpu::FilterMode::Nearest,
        ..Default::default()
    });

    let mut view = vec![];
    for level in 0..5 {
        let texture_view = texture.create_view(&wgpu::TextureViewDescriptor {
            label: Some("mip"),
            format: None,
            dimension: None,
            aspect: wgpu::TextureAspect::All,
            base_mip_level: level,
            mip_level_count: NonZeroU32::new(1),
            base_array_layer: 0,
            array_layer_count: None,
        });
        view.push(texture_view);
    }

    let shader = holder
        .device
        .create_shader_module(&wgpu::ShaderModuleDescriptor {
            label: None,
            source: wgpu::ShaderSource::Wgsl(Cow::Borrowed(include_str!("blit.wgsl"))),
        });
    let mut encoder = holder
        .device
        .create_command_encoder(&wgpu::CommandEncoderDescriptor { label: None });
    let pipeline = holder
        .device
        .create_render_pipeline(&wgpu::RenderPipelineDescriptor {
            label: Some("blit_1111"),
            layout: None,
            vertex: wgpu::VertexState {
                module: &shader,
                entry_point: "vs_main",
                buffers: &[],
            },
            fragment: Some(wgpu::FragmentState {
                module: &shader,
                entry_point: "fs_main",
                targets: &[TEXTURE_FORMAT.into()],
            }),
            primitive: wgpu::PrimitiveState {
                topology: wgpu::PrimitiveTopology::TriangleList,
                ..Default::default()
            },
            depth_stencil: None,
            multisample: wgpu::MultisampleState::default(),
            multiview: None,
        });
    let bind_group_layout = pipeline.get_bind_group_layout(0);
    for level in 1..5 {
        let bind_group = holder.device.create_bind_group(&wgpu::BindGroupDescriptor {
            layout: &bind_group_layout,
            entries: &[
                wgpu::BindGroupEntry {
                    binding: 0,
                    resource: wgpu::BindingResource::TextureView(&view[level as usize - 1]),
                },
                wgpu::BindGroupEntry {
                    binding: 1,
                    resource: wgpu::BindingResource::Sampler(&sampler),
                },
            ],
            label: None,
        });

        {
            let mut rpass = encoder.begin_render_pass(&wgpu::RenderPassDescriptor {
                label: None,
                color_attachments: &[wgpu::RenderPassColorAttachment {
                    view: &view[level],
                    resolve_target: None,
                    ops: wgpu::Operations {
                        load: wgpu::LoadOp::Clear(wgpu::Color::WHITE),
                        store: true,
                    },
                }],
                depth_stencil_attachment: None,
            });
            rpass.set_pipeline(&pipeline);
            rpass.set_bind_group(0, &bind_group, &[]);
            rpass.draw(0..3, 0..1);
        }
    }
    holder.queue.submit(std::iter::once(encoder.finish()));

    Texture {
        texture,
        view,
        sampler,
        width: dimensions.0,
        height: dimensions.1,
    }
}

//
// Created by 林炳河 on 2023/2/22.
//

#include "CanvasRenderingContextSkia.hpp"
#include "include/core/SkCanvas.h"
#include "include/core/SkColor.h"
#include "include/core/SkColorSpace.h"
#include "include/gpu/GrBackendSurface.h"
#include "include/gpu/GrDirectContext.h"
#include "include/gpu/gl/GrGLDefines.h"
#include "include/gpu/gl/GrGLInterface.h"
#include "include/gpu/gl/GrGLTypes.h"

struct ColorSettings {
    ColorSettings(sk_sp<SkColorSpace> colorSpace) {
        if (colorSpace == nullptr || colorSpace->isSRGB()) {
            colorType = kRGBA_8888_SkColorType;
            pixFormat = GR_GL_RGBA8;
        } else {
            colorType = kRGBA_F16_SkColorType;
            pixFormat = GR_GL_RGBA16F;
        }
    }
    SkColorType colorType;
    GrGLenum pixFormat;
};

CanvasRenderingContextSkia::CanvasRenderingContextSkia(int width, int height)
    : CanvasRenderingContextBase(), mCanvasWidth(width), mCanvasHeight(height) {
    auto interface = GrGLMakeNativeInterface();
    mContext = GrDirectContext::MakeGL(interface);

    makeSurfaceByPlatform();
}

CanvasRenderingContextSkia::~CanvasRenderingContextSkia() noexcept {}

void CanvasRenderingContextSkia::drawSomething() {
    auto canvas = mSurface->getCanvas();
    canvas->clear(SK_ColorBLACK);
    auto paint = SkPaint();
    paint.setColor(SkColorSetARGB(139, 228, 135, 255));  // greenish
    paint.setStyle(SkPaint::Style::kFill_Style);
    paint.setAntiAlias(true);
    canvas->drawOval(SkRect::MakeXYWH(100, 100, 800, 800), paint);
    mSurface->flushAndSubmit();
    printf("----- DrawSomething END -----\n");
}

#if IS_WEB
#include <emscripten/html5.h>
#include <webgl/webgl1.h>
void CanvasRenderingContextSkia::makeSurfaceByPlatform() {
    GrGLint sampleCnt;
    emscripten_glGetIntegerv(GL_SAMPLES, &sampleCnt);

    GrGLint stencil;
    emscripten_glGetIntegerv(GL_STENCIL_BITS, &stencil);
    // WebGL should already be clearing the color and stencil buffers, but do it again here to
    // ensure Skia receives them in the expected state.
    emscripten_glBindFramebuffer(GL_FRAMEBUFFER, 0);
    emscripten_glClearColor(0, 0, 0, 0);
    emscripten_glClearStencil(0);
    emscripten_glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    mContext->resetContext(kRenderTarget_GrGLBackendState | kMisc_GrGLBackendState);

    // The on-screen canvas is FBO 0. Wrap it in a Skia render target so Skia can render to it.
    GrGLFramebufferInfo info;
    info.fFBOID = 0;

    sk_sp<SkColorSpace> colorSpace = SkColorSpace::MakeSRGB();
    const auto colorSettings = ColorSettings(colorSpace);
    info.fFormat = colorSettings.pixFormat;
    GrBackendRenderTarget target(mCanvasWidth, mCanvasHeight, sampleCnt, stencil, info);
    sk_sp<SkSurface> surface(
        SkSurface::MakeFromBackendRenderTarget(mContext.get(), target, kBottomLeft_GrSurfaceOrigin,
                                               colorSettings.colorType, colorSpace, nullptr));
    mSurface = surface;
}
#else
void CanvasRenderingContextSkia::makeSurfaceByPlatform() {}
#endif

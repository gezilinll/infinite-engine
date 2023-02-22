#ifndef INFINITE_CANVAS_BINNDER
#define INFINITE_CANVAS_BINNDER

#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <emscripten/html5.h>
#include <webgl/webgl1.h>

#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"
#include "include/core/SkColor.h"
#include "include/core/SkColorSpace.h"
#include "include/gpu/GrDirectContext.h"
#include "include/gpu/gl/GrGLTypes.h"
#include "include/gpu/GrBackendSurface.h"
#include "include/gpu/gl/GrGLDefines.h"
#include "include/gpu/gl/GrGLInterface.h"

using namespace emscripten;

// Set the pixel format based on the colortype.
// These degrees of freedom are removed from canvaskit only to keep the interface simpler.
struct ColorSettings
{
    ColorSettings(sk_sp<SkColorSpace> colorSpace)
    {
        if (colorSpace == nullptr || colorSpace->isSRGB())
        {
            colorType = kRGBA_8888_SkColorType;
            pixFormat = GR_GL_RGBA8;
        }
        else
        {
            colorType = kRGBA_F16_SkColorType;
            pixFormat = GR_GL_RGBA16F;
        }
    }
    SkColorType colorType;
    GrGLenum pixFormat;
};

sk_sp<GrDirectContext> MakeGrContext()
{
    // We assume that any calls we make to GL for the remainder of this function will go to the
    // desired WebGL Context.
    // setup interface.
    auto interface = GrGLMakeNativeInterface();
    // setup context
    return GrDirectContext::MakeGL(interface);
}

sk_sp<SkSurface> MakeOnScreenGLSurface(sk_sp<GrDirectContext> dContext, int width, int height)
{
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
    dContext->resetContext(kRenderTarget_GrGLBackendState | kMisc_GrGLBackendState);

    // The on-screen canvas is FBO 0. Wrap it in a Skia render target so Skia can render to it.
    GrGLFramebufferInfo info;
    info.fFBOID = 0;

    sk_sp<SkColorSpace> colorSpace = SkColorSpace::MakeSRGB();
    const auto colorSettings = ColorSettings(colorSpace);
    info.fFormat = colorSettings.pixFormat;
    GrBackendRenderTarget target(width, height, sampleCnt, stencil, info);
    sk_sp<SkSurface> surface(SkSurface::MakeFromBackendRenderTarget(dContext.get(), target,
                                                                    kBottomLeft_GrSurfaceOrigin, colorSettings.colorType, colorSpace, nullptr));
    return surface;
}

void DrawSomething(sk_sp<SkSurface> surface)
{
    auto canvas = surface->getCanvas();
    canvas->clear(SK_ColorBLACK);
    auto paint = SkPaint();
    paint.setColor(SkColorSetARGB(139, 228, 135, 255)); // greenish
    paint.setStyle(SkPaint::Style::kFill_Style);
    paint.setAntiAlias(true);
    canvas->drawOval(SkRect::MakeXYWH(100, 100, 800, 800), paint);
    // for (int i = 0; i < 10; i++)
    // {
    //     auto x = std::rand() * 50;
    //     auto y = std::rand() * 50;
    //     canvas->drawOval(SkRect::MakeXYWH(x, y, 6, 6), paint);
    // }
    surface->flushAndSubmit();
    printf("----- DrawSomething END -----\n");
}

EMSCRIPTEN_BINDINGS(CanvasBinder)
{
    function("_MakeOnScreenGLSurface", select_overload<sk_sp<SkSurface>(sk_sp<GrDirectContext>, int, int)>(&MakeOnScreenGLSurface));
    function("_MakeGrContext", &MakeGrContext);
    function("DrawSomething", select_overload<void(sk_sp<SkSurface>)>(&DrawSomething));

    class_<GrDirectContext>("GrDirectContext")
        .smart_ptr<sk_sp<GrDirectContext>>("sk_sp<GrDirectContext>");
    class_<SkSurface>("Surface")
        .smart_ptr<sk_sp<SkSurface>>("sk_sp<Surface>");
}

#endif // INFINITE_CANVAS_BINNDER

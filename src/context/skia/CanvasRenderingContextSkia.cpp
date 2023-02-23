//
// Created by 林炳河 on 2023/2/22.
//

#include "CanvasRenderingContextSkia.hpp"
#include "SkiaUtils.hpp"
#include "include/core/SkCanvas.h"
#include "include/core/SkColor.h"
#include "include/core/SkColorSpace.h"
#include "include/core/SkPathEffect.h"
#include "include/effects/SkDashPathEffect.h"
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
    mPaint.setAntiAlias(true);
    mPaint.setStrokeMiter(10);
    mPaint.setStrokeCap(SkPaint::Cap::kButt_Cap);
    mPaint.setStrokeJoin(SkPaint::Join::kMiter_Join);
    mPaint.setStrokeWidth(mStrokeWidth);
    mPaint.setBlendMode(mGlobalCompositeOperation);

    auto interface = GrGLMakeNativeInterface();
    mContext = GrDirectContext::MakeGL(interface);
    makeSurfaceByPlatform();
}

CanvasRenderingContextSkia::~CanvasRenderingContextSkia() noexcept {}

void CanvasRenderingContextSkia::setLineDash(LineDash lineDash) { mLineDash = lineDash; }

void CanvasRenderingContextSkia::strokeRect(SkScalar x, SkScalar y, SkScalar width,
                                            SkScalar height) {
    auto strokePaint = getStrokePaint();

    //    auto shadowPaint = getShadowPaint();
    //    if (shadowPaint) {
    //        mSurface->getCanvas()->save();
    //        this._applyShadowOffsetMatrix();
    //        mSurface->getCanvas()->drawRect(SkRect::MakeXYWH(x, y, width, height),
    //        shadowPaint.get()); mSurface->getCanvas()->.restore();
    //    }
    mSurface->getCanvas()->drawRect(SkRect::MakeXYWH(x, y, width, height), strokePaint);
}

void CanvasRenderingContextSkia::fillRect(SkScalar x, SkScalar y, SkScalar width, SkScalar height) {
    auto fillPaint = getFillPaint();

    //    var shadowPaint = this._shadowPaint(fillPaint);
    //    if (shadowPaint) {
    //        this._canvas.save();
    //        this._applyShadowOffsetMatrix();
    //        this._canvas.drawRect(CanvasKit.XYWHRect(x, y, width, height), shadowPaint);
    //        this._canvas.restore();
    //        shadowPaint.dispose();
    //    }

    mSurface->getCanvas()->drawRect(SkRect::MakeXYWH(x, y, width, height), fillPaint);
}

SkPaint CanvasRenderingContextSkia::getStrokePaint() {
    SkPaint paint = mPaint;
    paint.setStyle(SkPaint::Style::kStroke_Style);
    if (mStrokeStyle.type == StrokeStyleType::Color) {
        paint.setColor(SkiaUtils::parseColorString(mStrokeStyle.color, mGlobalAlpha));
    } else {
        // TODO
    }
    paint.setStrokeWidth(mStrokeWidth);
    if (mLineDash.intervals.size() > 0) {
        paint.setPathEffect(SkDashPathEffect::Make(&mLineDash.intervals[0],
                                                   mLineDash.intervals.size(), mLineDash.phase));
    }
    return paint;
}

SkPaint CanvasRenderingContextSkia::getFillPaint() {
    SkPaint paint = mPaint;
    paint.setStyle(SkPaint::Style::kFill_Style);
    if (mStrokeStyle.type == StrokeStyleType::Color) {
        paint.setColor(SkiaUtils::parseColorString(mStrokeStyle.color, mGlobalAlpha));
    } else {
        // TODO
    }
    return paint;
}

void CanvasRenderingContextSkia::stroke() {
    SkPaint strokePaint = getStrokePaint();

    //    var shadowPaint = this._shadowPaint(strokePaint);
    //    if (shadowPaint) {
    //        this._canvas.save();
    //        this._applyShadowOffsetMatrix();
    //        this._canvas.drawPath(path, shadowPaint);
    //        this._canvas.restore();
    //        shadowPaint.dispose();
    //    }

    mSurface->getCanvas()->drawPath(mCurrentPath, strokePaint);
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

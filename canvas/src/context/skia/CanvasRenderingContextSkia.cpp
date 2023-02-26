//
// Created by 林炳河 on 2023/2/22.
//

#include "CanvasRenderingContextSkia.hpp"
#include "include/core/SkBlurTypes.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkColor.h"
#include "include/core/SkColorSpace.h"
#include "include/core/SkMaskFilter.h"
#include "include/core/SkPathEffect.h"
#include "include/core/SkTextBlob.h"
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

    mFont = SkFont(nullptr, 16);  // TODO default should be 10
    mFont.setSubpixel(true);

    auto interface = GrGLMakeNativeInterface();
    mContext = GrDirectContext::MakeGL(interface);
    makeSurfaceByPlatform();
}

CanvasRenderingContextSkia::~CanvasRenderingContextSkia() noexcept {}

void CanvasRenderingContextSkia::strokeRect(SkScalar x, SkScalar y, SkScalar width,
                                            SkScalar height) {
    auto strokePaint = getStrokePaint();
    SkPaint shadowPaint;
    if (initShadowPaintIfNeed(shadowPaint, strokePaint)) {
        mSurface->getCanvas()->save();
        applyShadowOffsetMatrix();
        mSurface->getCanvas()->drawRect(SkRect::MakeXYWH(x, y, width, height), shadowPaint);
        mSurface->getCanvas()->restore();
    }
    mSurface->getCanvas()->drawRect(SkRect::MakeXYWH(x, y, width, height), strokePaint);
}

void CanvasRenderingContextSkia::fillRect(SkScalar x, SkScalar y, SkScalar width, SkScalar height) {
    auto fillPaint = getFillPaint();

    SkPaint shadowPaint;
    if (initShadowPaintIfNeed(shadowPaint, fillPaint)) {
        mSurface->getCanvas()->save();
        applyShadowOffsetMatrix();
        mSurface->getCanvas()->drawRect(SkRect::MakeXYWH(x, y, width, height), shadowPaint);
        mSurface->getCanvas()->restore();
    }

    mSurface->getCanvas()->drawRect(SkRect::MakeXYWH(x, y, width, height), fillPaint);
}

void CanvasRenderingContextSkia::fillText(std::string text, SkScalar x, SkScalar y,
                                          SkScalar maxWidth) {
    // TODO do something with maxWidth, probably involving measure
    auto fillPaint = getFillPaint();
    auto blob = SkTextBlob::MakeFromString(text.data(), mFont, SkTextEncoding::kUTF8);

    SkPaint shadowPaint;
    if (initShadowPaintIfNeed(shadowPaint, fillPaint)) {
        mSurface->getCanvas()->save();
        applyShadowOffsetMatrix();
        mSurface->getCanvas()->drawTextBlob(blob, x, y, shadowPaint);
        mSurface->getCanvas()->restore();
    }

    mSurface->getCanvas()->drawTextBlob(blob, x, y, fillPaint);
}

SkPaint CanvasRenderingContextSkia::getStrokePaint() {
    SkPaint paint = mPaint;
    paint.setStyle(SkPaint::Style::kStroke_Style);
    if (mStrokeStyle.type == FillStrokeStyle::StyleType::Color) {
        paint.setColor(SkiaUtils::multiplyByAlpha(mStrokeStyle.color, mGlobalAlpha));
    } else {
        // TODO
    }
    paint.setStrokeWidth(mStrokeWidth);
    if (mLineDashList.size() > 0) {
        paint.setPathEffect(
            SkDashPathEffect::Make(&mLineDashList[0], mLineDashList.size(), mLineDashOffset));
    }
    return paint;
}

SkPaint CanvasRenderingContextSkia::getFillPaint() {
    SkPaint paint = mPaint;
    paint.setStyle(SkPaint::Style::kFill_Style);
    if (mFillStyle.type == FillStrokeStyle::StyleType::Color) {
        paint.setColor(SkiaUtils::multiplyByAlpha(mFillStyle.color, mGlobalAlpha));
    } else {
        // TODO
    }
    return paint;
}

bool CanvasRenderingContextSkia::initShadowPaintIfNeed(SkPaint& shadow, SkPaint& base) {
    // multiply first to see if the alpha channel goes to 0 after multiplication.
    SkColor alphaColor = SkiaUtils::multiplyByAlpha(mShadowColor, mGlobalAlpha);
    // if alpha is zero, no shadows
    if (!SkColorGetA(alphaColor)) {
        return false;
    }
    // one of these must also be non-zero (otherwise the shadow is
    // completely hidden.  And the spec says so).
    if (!(mShadowBlur || mShadowOffsetX || mShadowOffsetY)) {
        return false;
    }
    shadow = base;
    shadow.setColor(alphaColor);
    auto blurEffect
        = SkMaskFilter::MakeBlur(SkBlurStyle::kNormal_SkBlurStyle, mShadowBlur / 2, false);
    shadow.setMaskFilter(blurEffect);
    return true;
}

void CanvasRenderingContextSkia::applyShadowOffsetMatrix() {
    SkMatrix inverted = mCurrentTransform;
    mCurrentTransform.invert(&inverted);
    mSurface->getCanvas()->concat(inverted);
    mSurface->getCanvas()->concat(SkMatrix::Translate(mShadowOffsetX, mShadowOffsetY));
    mSurface->getCanvas()->concat(mCurrentTransform);
}

void CanvasRenderingContextSkia::arc(SkScalar x, SkScalar y, SkScalar radius, SkScalar startAngle,
                                     SkScalar endAngle, bool anticlockwise) {
    SkPath temp;
    SkScalar sweep = SkiaUtils::radiansToDegrees(endAngle - startAngle) - (360 * !anticlockwise);
    SkRect bounds = SkRect::MakeLTRB(x - radius, y - radius, x + radius, y + radius);
    temp.addArc(bounds, SkiaUtils::radiansToDegrees(startAngle), sweep);
    mCurrentPath.addPath(temp, SkPath::kExtend_AddPathMode);
}

void CanvasRenderingContextSkia::fill() {
    mCurrentPath.setFillType(SkPathFillType::kWinding);
    auto fillPaint = getFillPaint();
    SkPaint shadowPaint;
    if (initShadowPaintIfNeed(shadowPaint, fillPaint)) {
        mSurface->getCanvas()->save();
        applyShadowOffsetMatrix();
        mSurface->getCanvas()->drawPath(mCurrentPath, shadowPaint);
        mSurface->getCanvas()->restore();
    }
    mSurface->getCanvas()->drawPath(mCurrentPath, fillPaint);
}

void CanvasRenderingContextSkia::stroke() {
    SkPaint strokePaint = getStrokePaint();
    SkPaint shadowPaint;
    if (initShadowPaintIfNeed(shadowPaint, strokePaint)) {
        mSurface->getCanvas()->save();
        applyShadowOffsetMatrix();
        mSurface->getCanvas()->drawPath(mCurrentPath, shadowPaint);
        mSurface->getCanvas()->restore();
    }

    mSurface->getCanvas()->drawPath(mCurrentPath, strokePaint);
}

void CanvasRenderingContextSkia::clearRect(SkScalar x, SkScalar y, SkScalar width,
                                           SkScalar height) {
    mPaint.setStyle(SkPaint::Style::kFill_Style);
    mPaint.setBlendMode(SkBlendMode::kClear);
    mSurface->getCanvas()->drawRect(SkRect::MakeXYWH(x, y, width, height), mPaint);
    mPaint.setBlendMode(mGlobalCompositeOperation);
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

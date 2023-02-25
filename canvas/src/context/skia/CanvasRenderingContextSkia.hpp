//
// Created by 林炳河 on 2023/2/22.
//

#ifndef INFINITEENGINE_CANVASRENDERINGCONTEXTSKIA_HPP
#define INFINITEENGINE_CANVASRENDERINGCONTEXTSKIA_HPP

#include "CanvasRenderingContextBase.hpp"
#include "Macros.hpp"
#include "SkiaModels.hpp"
#include "SkiaUtils.hpp"
#include "include/core/SkBlendMode.h"
#include "include/core/SkFont.h"
#include "include/core/SkPaint.h"
#include "include/core/SkPath.h"
#include "include/core/SkSurface.h"

class CanvasRenderingContextSkia : public CanvasRenderingContextBase {
public:
    CanvasRenderingContextSkia(int width, int height);

    ~CanvasRenderingContextSkia() noexcept;

    void setLineWidth(SkScalar width) {
        if (width > 0) {
            mStrokeWidth = width;
        }
    }

    void setStrokeStyle(StrokeStyle style) { mStrokeStyle = style; }

    void setShadowOffsetX(SkScalar offset) { mShadowOffsetX = offset; }

    void setShadowOffsetY(SkScalar offset) { mShadowOffsetY = offset; }

    void setShadowColor(std::string color) { mShadowColor = SkiaUtils::parseColorString(color); }

    void setShadowBlur(SkScalar blurValue) {
        if (blurValue >= 0) {
            mShadowBlur = blurValue;
        }
    }

    void setLineDash(LineDash lineDash);

    void setGlobalAlpha(SkScalar alpha) {
        if (alpha < 0 || alpha > 1) {
            return;
        }
        mGlobalAlpha = alpha;
    }

    void setGlobalCompositeOperation(std::string operation) {
        try {
            auto result = SkiaUtils::parseBlendModeString(operation);
            mGlobalCompositeOperation = result;
        } catch (const char* msg) {
        }
    }

    void setFont(std::string fontStr) {
        try {
            auto result = SkiaUtils::parseFontString(fontStr);

        } catch (const char* msg) {
        }
    }

    void strokeRect(SkScalar x, SkScalar y, SkScalar width, SkScalar height);

    void fillRect(SkScalar x, SkScalar y, SkScalar width, SkScalar height);

    void beginPath() { mCurrentPath.reset(); }

    void moveTo(SkScalar x, SkScalar y) { mCurrentPath.moveTo(x, y); }

    void lineTo(SkScalar x, SkScalar y) { mCurrentPath.lineTo(x, y); }

    void closePath() { mCurrentPath.close(); }

    void stroke();

    void fillText(std::string text, SkScalar x, SkScalar y, SkScalar maxWidth = 0);

private:
    void makeSurfaceByPlatform();

    SkPaint getStrokePaint();

    SkPaint getFillPaint();

    bool initShadowPaintIfNeed(SkPaint& shadow, SkPaint& base);

    void applyShadowOffsetMatrix();

private:
    int mCanvasWidth;
    int mCanvasHeight;
    SkScalar mGlobalAlpha = 1;
    SkBlendMode mGlobalCompositeOperation = SkBlendMode::kSrcOver;

    StrokeStyle mStrokeStyle;
    LineDash mLineDash;
    SkScalar mStrokeWidth = 1;

    SkPath mCurrentPath;
    SkMatrix mCurrentTransform;

    Color mShadowColor = {0, 0, 0, 0};
    SkScalar mShadowOffsetX = 0;
    SkScalar mShadowOffsetY = 0;
    SkScalar mShadowBlur = 0;

    SkFont mFont;

    SkPaint mPaint;
    sk_sp<GrDirectContext> mContext;
    sk_sp<SkSurface> mSurface;
};

#endif  // INFINITEENGINE_CANVASRENDERINGCONTEXTSKIA_HPP

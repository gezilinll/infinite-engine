//
// Created by 林炳河 on 2023/2/22.
//

#ifndef INFINITEENGINE_CANVASRENDERINGCONTEXTSKIA_HPP
#define INFINITEENGINE_CANVASRENDERINGCONTEXTSKIA_HPP

#include "CanvasRenderingContextBase.hpp"
#include "FontManager.hpp"
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
    CanvasRenderingContextSkia(int width, int height, std::shared_ptr<FontManager> fontManager);

    ~CanvasRenderingContextSkia() noexcept;

    void setLineWidth(SkScalar width) {
        if (width > 0) {
            mStrokeWidth = width;
        }
    }

    void setLineCap(std::string cap) {
        if (cap == "butt") {
            mPaint.setStrokeCap(SkPaint::Cap::kButt_Cap);
        } else if (cap == "round") {
            mPaint.setStrokeCap(SkPaint::Cap::kRound_Cap);
        } else if (cap == "square") {
            mPaint.setStrokeCap(SkPaint::Cap::kSquare_Cap);
        }
    }

    void setMiterLimit(SkScalar value) {
        if (value > 0) {
            mPaint.setStrokeMiter(value);
        }
    }

    void setLineJoin(std::string join) {
        if (join == "miter") {
            mPaint.setStrokeJoin(SkPaint::Join::kMiter_Join);
        } else if (join == "round") {
            mPaint.setStrokeJoin(SkPaint::Join::kRound_Join);
        } else if (join == "bevel") {
            mPaint.setStrokeJoin(SkPaint::Join::kBevel_Join);
        }
    }

    void setLineDashOffset(SkScalar value) { mLineDashOffset = value; }

    void setStrokeStyle(FillStrokeStyle style) { mStrokeStyle = style; }

    void setFillStyle(FillStrokeStyle style) { mFillStyle = style; }

    void setShadowOffsetX(SkScalar offset) { mShadowOffsetX = offset; }

    void setShadowOffsetY(SkScalar offset) { mShadowOffsetY = offset; }

    void setShadowColor(std::string color) { mShadowColor = SkiaUtils::parseColorString(color); }

    void setShadowBlur(SkScalar blurValue) {
        if (blurValue >= 0) {
            mShadowBlur = blurValue;
        }
    }

    void setLineDash(std::vector<SkScalar> lineDash) {
        mLineDashList = lineDash;
        if (mLineDashList.size() % 2 == 1) {
            mLineDashList.insert(mLineDashList.end(), mLineDashList.begin(), mLineDashList.end());
        }
    }

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
            mPaint.setBlendMode(mGlobalCompositeOperation);
        } catch (std::string msg) {
            printf("setGlobalCompositeOperation failed. %s\n", msg.data());
        }
    }

    void setFont(std::string fontStr) {
        try {
            auto fontInfo = SkiaUtils::parseFontString(fontStr);
            auto typeFace = mFontManager->getTypeFaceFromCache(fontInfo);
            if (typeFace) {
                mFont.setSize(fontInfo.sizePx);
                mFont.setTypeface(typeFace);
            } else {
                printf("can not find target font: %s\n", fontStr.data());
            }
        } catch (std::string msg) {
            printf("setFont failed. %s\n", msg.data());
        }
    }

    void strokeRect(SkScalar x, SkScalar y, SkScalar width, SkScalar height);

    void fillRect(SkScalar x, SkScalar y, SkScalar width, SkScalar height);

    void arc(SkScalar x, SkScalar y, SkScalar radius, SkScalar startAngle, SkScalar endAngle,
             bool anticlockwise);

    void beginPath() { mCurrentPath.reset(); }

    void moveTo(SkScalar x, SkScalar y) { mCurrentPath.moveTo(x, y); }

    void lineTo(SkScalar x, SkScalar y) { mCurrentPath.lineTo(x, y); }

    void rect(SkScalar x, SkScalar y, SkScalar width, SkScalar height) {
        mCurrentPath.addRect(SkRect::MakeXYWH(x, y, width, height));
    }

    void closePath() { mCurrentPath.close(); }

    void stroke();

    void fill();

    void fillText(std::string text, SkScalar x, SkScalar y, SkScalar maxWidth = 0);

    void strokeText(std::string text, SkScalar x, SkScalar y, SkScalar maxWidth = 0);

    void clearRect(SkScalar x, SkScalar y, SkScalar width, SkScalar height);

    void flush() { mSurface->flushAndSubmit(); }

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

    FillStrokeStyle mStrokeStyle;
    FillStrokeStyle mFillStyle;
    SkScalar mLineDashOffset = 0;
    std::vector<SkScalar> mLineDashList;
    SkScalar mStrokeWidth = 1;

    SkPath mCurrentPath;
    SkMatrix mCurrentTransform;
    SkColor mShadowColor = SK_ColorTRANSPARENT;
    SkScalar mShadowOffsetX = 0;
    SkScalar mShadowOffsetY = 0;
    SkScalar mShadowBlur = 0;

    SkFont mFont;
    std::shared_ptr<FontManager> mFontManager;

    SkPaint mPaint;
    sk_sp<GrDirectContext> mContext;
    sk_sp<SkSurface> mSurface;
};

#endif  // INFINITEENGINE_CANVASRENDERINGCONTEXTSKIA_HPP

//
// Created by 林炳河 on 2023/2/22.
//

#ifndef INFINITEENGINE_CANVASRENDERINGCONTEXTSKIA_HPP
#define INFINITEENGINE_CANVASRENDERINGCONTEXTSKIA_HPP

#include "CanvasRenderingContextBase.hpp"
#include "Macros.hpp"
#include "SkiaModels.hpp"
#include "include/core/SkPaint.h"
#include "include/core/SkSurface.h"
#include "include/core/SkPath.h"
#include "include/core/SkBlendMode.h"

class CanvasRenderingContextSkia : public CanvasRenderingContextBase {
public:
    CanvasRenderingContextSkia(int width, int height);

    ~CanvasRenderingContextSkia() noexcept;

    void setLineWidth(SkScalar width) {
        mStrokeWidth = width;
        printf("setLineWidth %f\n", mStrokeWidth);
    }

    void setStrokeStyle(StrokeStyle style) { mStrokeStyle = style; }

    void strokeRect(SkScalar x, SkScalar y, SkScalar width, SkScalar height);

    void fillRect(SkScalar x, SkScalar y, SkScalar width, SkScalar height);

    void setLineDash(LineDash lineDash);

    void setShadowColor(std::string color) { mShadowColor = color; }

    void beginPath() { mCurrentPath.reset(); }

    void moveTo(SkScalar x, SkScalar y) { mCurrentPath.moveTo(x, y); }

    void lineTo(SkScalar x, SkScalar y) { mCurrentPath.lineTo(x, y); }

    void closePath() { mCurrentPath.close(); }

    void stroke();

private:
    void makeSurfaceByPlatform();

    SkPaint getStrokePaint();

    SkPaint getFillPaint();

    // Returns the shadow paint for the current settings or null if there
    // should be no shadow. This ends up being a copy of the given
    // paint with a blur maskfilter and the correct color.
    //    std::shared_ptr<SkPaint> getShadowPaint();

private:
    int mCanvasWidth;
    int mCanvasHeight;
    SkScalar mGlobalAlpha = 1;
    SkBlendMode mGlobalCompositeOperation = SkBlendMode::kSrcOver;

    StrokeStyle mStrokeStyle;
    LineDash mLineDash;
    SkScalar mStrokeWidth = 1;

    SkPath mCurrentPath;

    std::string mShadowColor = "#00000000";

    SkPaint mPaint;
    sk_sp<GrDirectContext> mContext;
    sk_sp<SkSurface> mSurface;
};

#endif  // INFINITEENGINE_CANVASRENDERINGCONTEXTSKIA_HPP

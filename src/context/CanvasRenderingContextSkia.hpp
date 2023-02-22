//
// Created by 林炳河 on 2023/2/22.
//

#ifndef INFINITEENGINE_CANVASRENDERINGCONTEXTSKIA_HPP
#define INFINITEENGINE_CANVASRENDERINGCONTEXTSKIA_HPP

#include "Macros.hpp"
#include "CanvasRenderingContextBase.hpp"
#include "include/core/SkSurface.h"

class CanvasRenderingContextSkia : public CanvasRenderingContextBase
{
public:
    CanvasRenderingContextSkia(int width, int height);

    ~CanvasRenderingContextSkia() noexcept;

    void drawSomething();

private:
    void makeSurfaceByPlatform();

private:
    int mCanvasWidth;
    int mCanvasHeight;
    sk_sp<GrDirectContext> mContext;
    sk_sp<SkSurface> mSurface;
};

#endif // INFINITEENGINE_CANVASRENDERINGCONTEXTSKIA_HPP

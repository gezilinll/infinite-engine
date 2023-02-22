#ifndef XMEDIA_XWEBUTILS_HPP
#define XMEDIA_XWEBUTILS_HPP

#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>

#include "SkSurface.h"

using namespace emscripten;

class SkiaTester
{
public:
    static void initLoader()
    {
        sk_sp<SkSurface> rasterSurface =
            SkSurface::MakeRasterN32Premul(100, 100);
        SkCanvas *rasterCanvas = rasterSurface->getCanvas();
    }
};

EMSCRIPTEN_BINDINGS(SkiaTester)
{
    class_<SkiaTester>("SkiaTester")
        .class_function("initLoader", &SkiaTester::initLoader);
}

#endif // XMEDIA_XWEBUTILS_HPP

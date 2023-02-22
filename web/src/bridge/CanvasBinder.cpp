#ifndef INFINITE_CANVAS_BINNDER
#define INFINITE_CANVAS_BINNDER

#include "Canvas.hpp"
#include "CanvasRenderingContextSkia.hpp"
#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <emscripten/html5.h>
#include <webgl/webgl1.h>

using namespace emscripten;

std::shared_ptr<Canvas> makeCanvas(int width, int height)
{
    return std::make_shared<Canvas>(width, height);
}

void DrawSomething()
{
    emscripten_glClearColor(0, 0, 0, 0);

    // auto canvas = surface->getCanvas();
    // canvas->clear(SK_ColorBLACK);
    // auto paint = SkPaint();
    // paint.setColor(SkColorSetARGB(139, 228, 135, 255)); // greenish
    // paint.setStyle(SkPaint::Style::kFill_Style);
    // paint.setAntiAlias(true);
    // canvas->drawOval(SkRect::MakeXYWH(100, 100, 800, 800), paint);
    // // for (int i = 0; i < 10; i++)
    // // {
    // //     auto x = std::rand() * 50;
    // //     auto y = std::rand() * 50;
    // //     canvas->drawOval(SkRect::MakeXYWH(x, y, 6, 6), paint);
    // // }
    // surface->flushAndSubmit();
    // printf("----- DrawSomething END -----\n");
}

EMSCRIPTEN_BINDINGS(CanvasBinder)
{
    function("makeCanvas", select_overload<std::shared_ptr<Canvas>(int, int)>(&makeCanvas));
    function("DrawSomething", &DrawSomething);

    class_<CanvasRenderingContextSkia>("CanvasRenderingContext2D")
        .smart_ptr<std::shared_ptr<CanvasRenderingContextSkia>>("std::shared_ptr<CanvasRenderingContext2D>")
        .function("drawSomething",
                  optional_override([](CanvasRenderingContextSkia &self) -> void
                                    { self.drawSomething(); }));

    class_<Canvas>("Canvas")
        .smart_ptr<std::shared_ptr<Canvas>>("std::shared_ptr<Canvas>")
        .function("get2DContext",
                  optional_override([](Canvas &self) -> std::shared_ptr<CanvasRenderingContextSkia>
                                    { 
                                        auto baseContext = self.getContext("2d");
                                        return std::dynamic_pointer_cast<CanvasRenderingContextSkia>(baseContext); }));
}

#endif // INFINITE_CANVAS_BINNDER

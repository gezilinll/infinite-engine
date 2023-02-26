#ifndef INFINITE_CANVAS_BINNDER
#define INFINITE_CANVAS_BINNDER

#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/html5.h>
#include <emscripten/val.h>
#include <webgl/webgl1.h>
#include "Canvas.hpp"
#include "CanvasRenderingContextSkia.hpp"
#include "SkiaUtils.hpp"

using namespace emscripten;

std::shared_ptr<Canvas> makeCanvas(int width, int height) {
    return std::make_shared<Canvas>(width, height);
}

void DrawSomething() {
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

EMSCRIPTEN_BINDINGS(CanvasBinder) {
    function("makeCanvas", select_overload<std::shared_ptr<Canvas>(int, int)>(&makeCanvas));
    function("DrawSomething", &DrawSomething);

    class_<CanvasRenderingContextSkia>("CanvasRenderingContext2D")
        .smart_ptr<std::shared_ptr<CanvasRenderingContextSkia>>(
            "std::shared_ptr<CanvasRenderingContext2D>")
        .function("setLineWidth",
                  optional_override([](CanvasRenderingContextSkia& self, float width) -> void {
                      self.setLineWidth(width);
                  }))
        .function("setShadowOffsetX",
                  optional_override([](CanvasRenderingContextSkia& self, float value) -> void {
                      self.setShadowOffsetX(value);
                  }))
        .function("setShadowOffsetY",
                  optional_override([](CanvasRenderingContextSkia& self, float value) -> void {
                      self.setShadowOffsetY(value);
                  }))
        .function("setShadowBlur",
                  optional_override([](CanvasRenderingContextSkia& self, float value) -> void {
                      self.setShadowBlur(value);
                  }))
        .function("setShadowColor",
                  optional_override([](CanvasRenderingContextSkia& self,
                                       std::string value) -> void { self.setShadowColor(value); }))
        .function("setStrokeStyleColor", optional_override([](CanvasRenderingContextSkia& self,
                                                              std::string value) -> void {
                      FillStrokeStyle result;
                      result.color = SkiaUtils::parseColorString(value);
                      self.setStrokeStyle(result);
                  }))
        .function("setFillStyleColor", optional_override([](CanvasRenderingContextSkia& self,
                                                            std::string value) -> void {
                      FillStrokeStyle result;
                      result.color = SkiaUtils::parseColorString(value);
                      self.setFillStyle(result);
                  }))
        // .function("setDirection",
        //           optional_override([](CanvasRenderingContextSkia& self,
        //                                std::string value) -> void { self.setDirection(value); }))
        .function("setGlobalAlpha",
                  optional_override([](CanvasRenderingContextSkia& self, float value) -> void {
                      self.setGlobalAlpha(value);
                  }))
        .function(
            "setGlobalCompositeOperation",
            optional_override([](CanvasRenderingContextSkia& self, std::string value) -> void {
                self.setGlobalCompositeOperation(value);
            }))
        .function("arc", optional_override([](CanvasRenderingContextSkia& self, float x, float y,
                                              float radius, float startAngle, float endAngle,
                                              bool anticlockwise) -> void {
                      self.arc(x, y, radius, startAngle, endAngle, anticlockwise);
                  }))
        .function("fill",
                  optional_override([](CanvasRenderingContextSkia& self) -> void { self.fill(); }))
        .function("strokeRect",
                  optional_override([](CanvasRenderingContextSkia& self, float x, float y, float w,
                                       float h) -> void { self.strokeRect(x, y, w, h); }))
        .function("fillRect",
                  optional_override([](CanvasRenderingContextSkia& self, float x, float y, float w,
                                       float h) -> void { self.fillRect(x, y, w, h); }))
        .function("fillText",
                  optional_override(
                      [](CanvasRenderingContextSkia& self, std::string text, float x, float y,
                         float maxWidth) -> void { self.fillText(text, x, y, maxWidth); }))
        .function("beginPath", optional_override([](CanvasRenderingContextSkia& self) -> void {
                      self.beginPath();
                  }))
        .function("closePath", optional_override([](CanvasRenderingContextSkia& self) -> void {
                      self.closePath();
                  }))
        .function("moveTo", optional_override([](CanvasRenderingContextSkia& self, float x,
                                                 float y) -> void { self.moveTo(x, y); }))
        .function("lineTo", optional_override([](CanvasRenderingContextSkia& self, float x,
                                                 float y) -> void { self.lineTo(x, y); }))
        .function("stroke", optional_override(
                                [](CanvasRenderingContextSkia& self) -> void { self.stroke(); }));

    class_<Canvas>("Canvas")
        .smart_ptr<std::shared_ptr<Canvas>>("std::shared_ptr<Canvas>")
        .function(
            "get2DContext",
            optional_override([](Canvas& self) -> std::shared_ptr<CanvasRenderingContextSkia> {
                auto baseContext = self.getContext("2d");
                return std::dynamic_pointer_cast<CanvasRenderingContextSkia>(baseContext);
            }));
}

#endif  // INFINITE_CANVAS_BINNDER

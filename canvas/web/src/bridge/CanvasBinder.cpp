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
#include "include/core/SkAlphaType.h"
#include "include/core/SkColorSpace.h"
#include "include/core/SkColorType.h"
#include "include/core/SkData.h"

using namespace emscripten;
using WASMPointerF32 = uintptr_t;
using WASMPointerU8 = uintptr_t;

std::shared_ptr<Canvas> makeCanvas(int width, int height) {
    return std::make_shared<Canvas>(width, height);
}

void DrawSomething() { emscripten_glClearColor(0, 0, 0, 0); }

EMSCRIPTEN_BINDINGS(CanvasBinder) {
    function("makeCanvas", select_overload<std::shared_ptr<Canvas>(int, int)>(&makeCanvas));
    function("DrawSomething", &DrawSomething);

    class_<CanvasRenderingContextSkia>("CanvasRenderingContext2D")
        .smart_ptr<std::shared_ptr<CanvasRenderingContextSkia>>(
            "std::shared_ptr<CanvasRenderingContext2D>")
        .function("setLineDash", optional_override([](CanvasRenderingContextSkia& self,
                                                      WASMPointerF32 cptr, int count) -> void {
                      const float* intervals = reinterpret_cast<const float*>(cptr);
                      std::vector<SkScalar> result;
                      for (int i = 0; i < count; i++) {
                          result.push_back(intervals[i]);
                      }
                      self.setLineDash(result);
                  }))
        .function("setLineDashOffset",
                  optional_override([](CanvasRenderingContextSkia& self, float offset) -> void {
                      self.setLineDashOffset(offset);
                  }))
        .function("setLineCap",
                  optional_override([](CanvasRenderingContextSkia& self, std::string cap) -> void {
                      self.setLineCap(cap);
                  }))
        .function("setLineJoin",
                  optional_override([](CanvasRenderingContextSkia& self, std::string join) -> void {
                      self.setLineJoin(join);
                  }))
        .function("setLineWidth",
                  optional_override([](CanvasRenderingContextSkia& self, float width) -> void {
                      self.setLineWidth(width);
                  }))
        .function("setMiterLimit",
                  optional_override([](CanvasRenderingContextSkia& self, float value) -> void {
                      self.setMiterLimit(value);
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
        .function("setFont",
                  optional_override([](CanvasRenderingContextSkia& self,
                                       std::string value) -> void { self.setFont(value); }))
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
        .function("strokeText",
                  optional_override(
                      [](CanvasRenderingContextSkia& self, std::string text, float x, float y,
                         float maxWidth) -> void { self.strokeText(text, x, y, maxWidth); }))
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
        .function("rect",
                  optional_override([](CanvasRenderingContextSkia& self, float x, float y, float w,
                                       float h) -> void { self.rect(x, y, w, h); }))
        .function("stroke", optional_override(
                                [](CanvasRenderingContextSkia& self) -> void { self.stroke(); }))
        .function("clearRect", optional_override([](CanvasRenderingContextSkia& self, float x,
                                                    float y, float width, float height) -> void {
                      self.clearRect(x, y, width, height);
                  }))
        .function("readPixels", optional_override([](CanvasRenderingContextSkia& self) -> long {
                      return self.readPixels();
                  }))
        .function("drawImageWithoutClip",
                  optional_override([](CanvasRenderingContextSkia& self, WASMPointerU8 pPtr,
                                       int width, int height, int plen, size_t rowBytes, int dx,
                                       int dy, int dWidth, int dHeight) -> void {
                      uint8_t* imgData = reinterpret_cast<uint8_t*>(pPtr);
                      SkImageInfo info = SkImageInfo::Make(
                          width, height, SkColorType::kRGBA_8888_SkColorType,
                          SkAlphaType::kUnpremul_SkAlphaType, SkColorSpace::MakeSRGB());
                      sk_sp<SkData> pixelData = SkData::MakeFromMalloc(imgData, plen);
                      self.drawImage(SkImage::MakeRasterData(info, pixelData, rowBytes), dx, dy,
                                     dWidth, dHeight);
                  }))
        .function(
            "drawImageWithClip",
            optional_override([](CanvasRenderingContextSkia& self, WASMPointerU8 pPtr, int width,
                                 int height, int plen, size_t rowBytes, int sx, int sy, int sWidth,
                                 int sHeight, int dx, int dy, int dWidth, int dHeight) -> void {
                uint8_t* imgData = reinterpret_cast<uint8_t*>(pPtr);
                SkImageInfo info = SkImageInfo::Make(
                    width, height, SkColorType::kRGBA_8888_SkColorType,
                    SkAlphaType::kUnpremul_SkAlphaType, SkColorSpace::MakeSRGB());
                sk_sp<SkData> pixelData = SkData::MakeFromMalloc(imgData, plen);
                self.drawImage(SkImage::MakeRasterData(info, pixelData, rowBytes), sx, sy, sWidth,
                               sHeight, dx, dy, dWidth, dHeight);
            }));

    class_<Canvas>("Canvas")
        .smart_ptr<std::shared_ptr<Canvas>>("std::shared_ptr<Canvas>")
        .function(
            "get2DContext",
            optional_override([](Canvas& self) -> std::shared_ptr<CanvasRenderingContextSkia> {
                auto baseContext = self.getContext("2d");
                return std::dynamic_pointer_cast<CanvasRenderingContextSkia>(baseContext);
            }))
        .function("flush", optional_override([](Canvas& self) -> void { self.flush(); }))
        .function("loadFont", optional_override([](Canvas& self, WASMPointerU8 fPtr, int byteLength,
                                                   std::string family, std::string style,
                                                   std::string weight) -> void {
                      uint8_t* font = reinterpret_cast<uint8_t*>(fPtr);
                      FontInfo info;
                      info.family = family;
                      info.style = style;
                      info.weight = weight;
                      self.loadFont(font, byteLength, info);
                  }));
}

#endif  // INFINITE_CANVAS_BINNDER

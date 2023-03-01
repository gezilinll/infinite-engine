#ifndef INFINITE_INFINITE_BINNDER
#define INFINITE_INFINITE_BINNDER

#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/html5.h>
#include <emscripten/val.h>
#include <webgl/webgl1.h>
#include "ImageElement.hpp"
#include "InfiniteEngine.hpp"
#include "include/core/SkAlphaType.h"
#include "include/core/SkColorSpace.h"
#include "include/core/SkColorType.h"
#include "include/core/SkData.h"

using namespace emscripten;
using WASMPointerU8 = uintptr_t;

std::shared_ptr<InfiniteEngine> makeEngine(int width, int height) {
    return std::make_shared<InfiniteEngine>(width, height);
}

std::shared_ptr<ImageElement> makeImageElement(int id) {
    return std::make_shared<ImageElement>(id);
}

void DrawSomething() { emscripten_glClearColor(0, 0, 0, 0); }

EMSCRIPTEN_BINDINGS(InfiniteBinding) {
    function("makeEngine", select_overload<std::shared_ptr<InfiniteEngine>(int, int)>(&makeEngine));
    function("makeImageElement",
             select_overload<std::shared_ptr<ImageElement>(int)>(&makeImageElement));
    function("DrawSomething", &DrawSomething);

    class_<InfiniteEngine>("InfiniteEngine")
        .smart_ptr<std::shared_ptr<InfiniteEngine>>("std::shared_ptr<InfiniteEngine>")
        .function("loadFont", optional_override([](InfiniteEngine& self, WASMPointerU8 fPtr,
                                                   int byteLength, std::string family,
                                                   std::string style, std::string weight) -> void {
                      uint8_t* font = reinterpret_cast<uint8_t*>(fPtr);
                      FontInfo info;
                      info.family = family;
                      info.style = style;
                      info.weight = weight;
                      self.loadFont(font, byteLength, info);
                  }))
        .function("addImageElement",
                  optional_override([](InfiniteEngine& self, std::shared_ptr<ImageElement>& element)
                                        -> void { self.addElement(element); }))
        .function("requestRenderFrame", optional_override([](InfiniteEngine& self) -> bool {
                      return self.requestRenderFrame();
                  }))
        .function("readPixels", optional_override([](InfiniteEngine& self) -> long {
                      return self.readPixels();
                  }));

    class_<Element>("Element").smart_ptr<std::shared_ptr<Element>>("std::shared_ptr<Element>");

    class_<ImageElement>("ImageElement")
        .smart_ptr<std::shared_ptr<ImageElement>>("std::shared_ptr<ImageElement>")
        .function("setSource",
                  optional_override([](ImageElement& self, WASMPointerU8 pPtr, int width,
                                       int height, int plen, size_t rowBytes) -> void {
                      uint8_t* imgData = reinterpret_cast<uint8_t*>(pPtr);
                      SkImageInfo info = SkImageInfo::Make(
                          width, height, SkColorType::kRGBA_8888_SkColorType,
                          SkAlphaType::kUnpremul_SkAlphaType, SkColorSpace::MakeSRGB());
                      sk_sp<SkData> pixelData = SkData::MakeFromMalloc(imgData, plen);
                      self.setSource(SkImage::MakeRasterData(info, pixelData, rowBytes));
                  }))
        .function("setSrcRect", optional_override([](ImageElement& self, float x, float y,
                                                     float width, float height) -> void {
                      self.setSrcRect(x, y, width, height);
                  }))
        .function("setDstRect", optional_override([](ImageElement& self, float x, float y,
                                                     float width, float height) -> void {
                      self.setDstRect(x, y, width, height);
                  }));
}

#endif  // INFINITE_INFINITE_BINNDER

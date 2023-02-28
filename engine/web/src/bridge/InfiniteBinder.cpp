#ifndef INFINITE_INFINITE_BINNDER
#define INFINITE_INFINITE_BINNDER

#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/html5.h>
#include <emscripten/val.h>
#include <webgl/webgl1.h>
#include "InfiniteEngine.hpp"

using namespace emscripten;
using WASMPointerU8 = uintptr_t;

std::shared_ptr<InfiniteEngine> makeEngine(int width, int height) {
    return std::make_shared<InfiniteEngine>(width, height);
}

void DrawSomething() { emscripten_glClearColor(0, 0, 0, 0); }

EMSCRIPTEN_BINDINGS(InfiniteBinding) {
    function("makeEngine", select_overload<std::shared_ptr<InfiniteEngine>(int, int)>(&makeEngine));
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
                  }));
}

#endif  // INFINITE_INFINITE_BINNDER

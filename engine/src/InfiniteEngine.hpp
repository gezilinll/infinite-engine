//
// Created by 林炳河 on 2023/2/28.
//

#ifndef ENGINE_INFINITEENGINE_HPP
#define ENGINE_INFINITEENGINE_HPP

#include <memory>
#include "Canvas.hpp"
#include "Models.hpp"
#include "CanvasRenderingContextSkia.hpp"

class InfiniteEngine {
public:
    InfiniteEngine(int width, int height);

    ~InfiniteEngine();

    void loadFont(const void* data, size_t length, const FontInfo& info);

private:
    std::shared_ptr<Canvas> mCanvas;
    std::shared_ptr<CanvasRenderingContextSkia> mContext;
};

#endif  // ENGINE_INFINITEENGINE_HPP

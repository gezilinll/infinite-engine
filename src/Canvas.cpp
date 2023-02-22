#include "Canvas.hpp"
#include "CanvasRenderingContextSkia.hpp"

Canvas::Canvas(int width, int height) : mWidth(width), mHeight(height), mContextSkia(nullptr) {}

Canvas::~Canvas() {}

std::shared_ptr<CanvasRenderingContextBase> Canvas::getContext(std::string typeName) {
    if (typeName == "2d") {
        if (!mContextSkia) {
            mContextSkia = std::make_shared<CanvasRenderingContextSkia>(mWidth, mHeight);
        }
        return mContextSkia;
    } else {
        return nullptr;
    }
}
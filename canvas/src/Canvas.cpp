#include "Canvas.hpp"
#include "CanvasRenderingContextSkia.hpp"

Canvas::Canvas(int width, int height) : mWidth(width), mHeight(height), mContextSkia(nullptr) {
    mFontManager = std::make_shared<FontManager>();
}

Canvas::~Canvas() {}

std::shared_ptr<CanvasRenderingContextBase> Canvas::getContext(std::string typeName) {
    if (typeName == "2d") {
        if (!mContextSkia) {
            mContextSkia = std::make_shared<CanvasRenderingContextSkia>(mWidth, mHeight, mFontManager);
        }
        return mContextSkia;
    } else {
        return nullptr;
    }
}

void Canvas::loadFont(const void *data, size_t length, const FontInfo &info) {
    mFontManager->loadFont(data, length, info);
}



//
// Created by 林炳河 on 2023/2/28.
//

#include "InfiniteEngine.hpp"

InfiniteEngine::InfiniteEngine(int width, int height) {
    mCanvas = std::make_shared<Canvas>(width, height);
    mContext = std::dynamic_pointer_cast<CanvasRenderingContextSkia>(mCanvas->getContext("2d"));
}

InfiniteEngine::~InfiniteEngine() {}

void InfiniteEngine::loadFont(const void* data, size_t length, const FontInfo& info) {
    mCanvas->loadFont(data, length, info);
}

void InfiniteEngine::addElement(std::shared_ptr<Element> element) { mElements.push_back(element); }

bool InfiniteEngine::requestRenderFrame() {
    bool frameUpdated = false;
    for (auto element : mElements) {
        frameUpdated = element->requestRender(mContext) || frameUpdated;
    }
    if (frameUpdated) {
        mContext->flush();
    }
    return frameUpdated;
}
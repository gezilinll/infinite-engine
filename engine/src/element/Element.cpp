//
// Created by 林炳河 on 2023/2/28.
//

#include "Element.hpp"

Element::Element(int32_t id) : mID(id), mSceneTree(nullptr) {}

Element::~Element() {}

void Element::registerStatusObserver(StatusObserver observer) { mStatusObserver = observer; }

void Element::invokeStatusObserver() {
    if (mStatusObserver) {
        mStatusObserver(shared_from_this());
    }
}

SkRect Element::getRectToClear() {
    return SkRect::MakeEmpty();
}

SkRect Element::getRectToDraw() {
    return SkRect::MakeEmpty();
}

void Element::requestRender(std::shared_ptr<CanvasRenderingContextSkia> context) {}

void Element::requestRenderDirty(std::shared_ptr<CanvasRenderingContextSkia> context, const SkRect &dirtyRect) {}
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

void Element::requestRender(std::shared_ptr<CanvasRenderingContextSkia> context) {}
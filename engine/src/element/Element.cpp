//
// Created by 林炳河 on 2023/2/28.
//

#include "Element.hpp"

Element::Element(int32_t id) : mId(id) {}

Element::~Element() {}

bool Element::requestRender(std::shared_ptr<CanvasRenderingContextSkia> context) { return false; }
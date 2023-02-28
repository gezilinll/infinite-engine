//
// Created by 林炳河 on 2023/2/28.
//

#ifndef INFINITEENGINE_ELEMENT_HPP
#define INFINITEENGINE_ELEMENT_HPP

#include "CanvasRenderingContextSkia.hpp"

class Element {
public:
    Element(int32_t id);

    virtual ~Element();

    virtual void requestRender(std::shared_ptr<CanvasRenderingContextSkia> context);

protected:
    int32_t mId;
};

#endif  // INFINITEENGINE_ELEMENT_HPP

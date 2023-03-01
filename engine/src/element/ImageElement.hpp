//
// Created by 林炳河 on 2023/2/28.
//

#ifndef INFINITEENGINE_IMAGEELEMENT_HPP
#define INFINITEENGINE_IMAGEELEMENT_HPP

#include "Element.hpp"
#include "include/core/SkImage.h"

class ImageElement : public Element {
public:
    ImageElement(int32_t id);

    ~ImageElement();

    void setSource(sk_sp<SkImage> image);

    void setSrcRect(SkScalar x, SkScalar y, SkScalar width, SkScalar height);

    void setDstRect(SkScalar x, SkScalar y, SkScalar width, SkScalar height);

    bool requestRender(std::shared_ptr<CanvasRenderingContextSkia> context);

private:
    SkRect mSrcRect;
    SkRect mDstRect;
    bool mHasRendered;
    sk_sp<SkImage> mImageData;
};

#endif  // INFINITEENGINE_IMAGEELEMENT_HPP

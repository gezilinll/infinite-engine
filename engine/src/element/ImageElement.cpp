//
// Created by 林炳河 on 2023/2/28.
//

#include "ImageElement.hpp"

ImageElement::ImageElement(int32_t id) : Element(id), mHasRendered(false) {}

ImageElement::~ImageElement() {}

void ImageElement::setSource(sk_sp<SkImage> image) { mImageData = image; }

void ImageElement::setSrcRect(SkScalar x, SkScalar y, SkScalar width, SkScalar height) {
    mSrcRect = SkRect::MakeXYWH(x, y, width, height);
}

void ImageElement::setDstRect(SkScalar x, SkScalar y, SkScalar width, SkScalar height) {
    mDstRect = SkRect::MakeXYWH(x, y, width, height);
}

bool ImageElement::requestRender(std::shared_ptr<CanvasRenderingContextSkia> context) {
    if (mImageData && !mHasRendered) {
        SkScalar sx, sy, sw, sh, dx, dy, dw, dh;
        sx = mSrcRect.x();
        sy = mSrcRect.y();
        sw = mSrcRect.width() > 0 ? mSrcRect.width() : mImageData->width();
        sh = mSrcRect.height() > 0 ? mSrcRect.height() : mImageData->height();
        dx = mDstRect.x();
        dy = mDstRect.y();
        dw = mDstRect.width() > 0 ? mDstRect.width() : mImageData->width();
        dh = mDstRect.height() > 0 ? mDstRect.height() : mImageData->height();
        context->drawImage(mImageData, sx, sy, sw, sh, dx, dy, dw, dh);
        mHasRendered = true;
        return true;
    }
    return false;
}
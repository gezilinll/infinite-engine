//
// Created by 林炳河 on 2023/2/28.
//

#include "ImageElement.hpp"

ImageElement::ImageElement(int32_t id) : Element(id) {}

ImageElement::~ImageElement() {}

void ImageElement::bindSceneTree(std::shared_ptr<SceneTree> sceneTree) {
    Element::bindSceneTree(sceneTree);
    updateSceneInfo(false);
}

void ImageElement::updateSceneInfo(bool removed) {
    if (removed) {
        // TODO
    } else if (mSceneTree && mImageData) {
        float min[2] = {mStatus.dstRect.x(), mStatus.dstRect.y()};
        float max[2]
            = {mStatus.dstRect.x()
                   + (mStatus.dstRect.width() ? mStatus.dstRect.width() : mImageData->width()),
               mStatus.dstRect.y()
                   + (mStatus.dstRect.height() ? mStatus.dstRect.height() : mImageData->height())};
        if (mStatus.isInScene) {
            float lastMin[2] = {mStatus.rectInScene.x(), mStatus.rectInScene.y()};
            float lastMax[2] = {mStatus.rectInScene.x() + mStatus.rectInScene.width(),
                                mStatus.rectInScene.y() + mStatus.rectInScene.height()};
            mSceneTree->Remove(&lastMin[0], &lastMax[0], mID);
        }
        mSceneTree->Insert(&min[0], &max[0], mID);
        mStatus.rectInScene = SkRect::MakeXYWH(min[0], min[1], max[0] - min[0], max[1] - min[1]);
    }
}

void ImageElement::setSource(sk_sp<SkImage> image) {
    mImageData = image;
    Element::invokeStatusObserver();
    updateSceneInfo(false);
}

void ImageElement::setSrcRect(SkScalar x, SkScalar y, SkScalar width, SkScalar height) {
    mStatus.srcRect = SkRect::MakeXYWH(x, y, width, height);
    Element::invokeStatusObserver();
}

void ImageElement::setDstRect(SkScalar x, SkScalar y, SkScalar width, SkScalar height) {
    mStatus.dstRect = SkRect::MakeXYWH(x, y, width, height);
    Element::invokeStatusObserver();
    updateSceneInfo(false);
}

void ImageElement::requestRender(std::shared_ptr<CanvasRenderingContextSkia> context) {
    if (mImageData) {
        SkScalar sx, sy, sw, sh, dx, dy, dw, dh;
        sx = mStatus.srcRect.x();
        sy = mStatus.srcRect.y();
        sw = mStatus.srcRect.width() > 0 ? mStatus.srcRect.width() : mImageData->width();
        sh = mStatus.srcRect.height() > 0 ? mStatus.srcRect.height() : mImageData->height();
        dx = mStatus.dstRect.x();
        dy = mStatus.dstRect.y();
        dw = mStatus.dstRect.width() > 0 ? mStatus.dstRect.width() : mImageData->width();
        dh = mStatus.dstRect.height() > 0 ? mStatus.dstRect.height() : mImageData->height();
        if (mStatus.dstRect.width() <= 0 || mStatus.dstRect.height() < 0) {
            mStatus.dstRect = SkRect::MakeXYWH(dx, dy, dw, dh);
        }
        context->drawImage(mImageData, sx, sy, sw, sh, dx, dy, dw, dh);
        mStatus.lastDstRect = mStatus.dstRect;
    }
}
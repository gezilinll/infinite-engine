//
// Created by 林炳河 on 2023/2/28.
//

#ifndef INFINITEENGINE_IMAGEELEMENT_HPP
#define INFINITEENGINE_IMAGEELEMENT_HPP

#include "Element.hpp"
#include "include/core/SkImage.h"

struct ImageElementStatus : public ElementStatus {
    bool isInScene = false;
    SkRect rectInScene;
    SkRect lastDstRect;
    SkRect srcRect;
    SkRect dstRect;
};

class ImageElement : public Element {
public:
    ImageElement(int32_t id);

    ~ImageElement();

    void bindSceneTree(std::shared_ptr<SceneTree> sceneTree) override;

    void setSource(sk_sp<SkImage> image);

    SkRect getRectToClear() override;

    SkRect getRectToDraw() override;

    void setSrcRect(SkScalar x, SkScalar y, SkScalar width, SkScalar height);

    void setDstRect(SkScalar x, SkScalar y, SkScalar width, SkScalar height);

    void requestRender(std::shared_ptr<CanvasRenderingContextSkia> context) override;

    void requestRenderDirty(std::shared_ptr<CanvasRenderingContextSkia> context, const SkRect &dirtyRect) override;
private:
    void updateSceneInfo(bool removed);

private:
    ImageElementStatus mStatus;
    sk_sp<SkImage> mImageData;
};

#endif  // INFINITEENGINE_IMAGEELEMENT_HPP

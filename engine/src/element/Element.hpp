//
// Created by 林炳河 on 2023/2/28.
//

#ifndef INFINITEENGINE_ELEMENT_HPP
#define INFINITEENGINE_ELEMENT_HPP

#include <functional>
#include "CanvasRenderingContextSkia.hpp"
#include "RTree.h"

struct ElementStatus {
    bool isInScene = false;
};

class Element : public std::enable_shared_from_this<Element> {
public:
    typedef std::function<void(std::shared_ptr<Element>)> StatusObserver;

    Element(int32_t id);

    virtual ~Element();

    int32_t getID() { return mID; }

    void registerStatusObserver(StatusObserver observer);

    virtual SkRect getRectToClear();

    virtual SkRect getRectToDraw();

    virtual void requestRender(std::shared_ptr<CanvasRenderingContextSkia> context);

    virtual void requestRenderDirty(std::shared_ptr<CanvasRenderingContextSkia> context,
                                    const SkRect& dirtyRect);

    virtual void bindSceneTree(std::shared_ptr<SceneTree> sceneTree) { mSceneTree = sceneTree; }

protected:
    void invokeStatusObserver();

protected:
    std::shared_ptr<SceneTree> mSceneTree;
    int32_t mID;

private:
    StatusObserver mStatusObserver;
};

#endif  // INFINITEENGINE_ELEMENT_HPP

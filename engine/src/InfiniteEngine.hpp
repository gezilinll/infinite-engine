//
// Created by 林炳河 on 2023/2/28.
//

#ifndef ENGINE_INFINITEENGINE_HPP
#define ENGINE_INFINITEENGINE_HPP

#include <memory>
#include "Canvas.hpp"
#include "CanvasRenderingContextSkia.hpp"
#include "Element.hpp"
#include "Models.hpp"
#include "RTree.h"

struct EngineStatus {
    bool sceneLineEnabled = false;
};

class InfiniteEngine {
public:
    InfiniteEngine(int width, int height);

    ~InfiniteEngine();

    void loadFont(const void* data, size_t length, const FontInfo& info);

    void addElement(std::shared_ptr<Element> element);

    bool requestRenderFrame();

    long readPixels() { return mContext->readPixels(); };

    void enableDrawScene() { mCurrentStatus.sceneLineEnabled = true; }

    void disableDrawScene() { mCurrentStatus.sceneLineEnabled = false; }

private:
    void onElementStatusChanged(std::shared_ptr<Element> element);

    bool ifEngineStatusChanged();

private:
    std::shared_ptr<SceneTree> mSceneTree;
    std::vector<std::shared_ptr<Element>> mElements;
    std::vector<std::shared_ptr<Element>> mAddedElements;
    std::unordered_map<int32_t, std::shared_ptr<Element>> mAddedElementMap;
    std::unordered_map<int32_t, std::shared_ptr<Element>> mChangedElements;

    std::shared_ptr<Canvas> mCanvas;
    std::shared_ptr<CanvasRenderingContextSkia> mContext;
    EngineStatus mLastStatus;
    EngineStatus mCurrentStatus;
};

#endif  // ENGINE_INFINITEENGINE_HPP

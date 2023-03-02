//
// Created by 林炳河 on 2023/2/28.
//

#include "InfiniteEngine.hpp"

InfiniteEngine::InfiniteEngine(int width, int height) {
    mCanvas = std::make_shared<Canvas>(width, height);
    mContext = std::dynamic_pointer_cast<CanvasRenderingContextSkia>(mCanvas->getContext("2d"));
    mSceneTree = std::make_shared<SceneTree>();
}

InfiniteEngine::~InfiniteEngine() {}

void InfiniteEngine::loadFont(const void* data, size_t length, const FontInfo& info) {
    mCanvas->loadFont(data, length, info);
}

void InfiniteEngine::onElementStatusChanged(std::shared_ptr<Element> element) {
    if (mAddedElementMap.find(element->getID()) != mAddedElementMap.end()) {
        return;
    }
    mChangedElements.insert(std::make_pair(element->getID(), element));
}

void InfiniteEngine::addElement(std::shared_ptr<Element> element) {
    mElements.push_back(element);
    mAddedElementMap.insert(std::make_pair(element->getID(), element));
    mAddedElements.push_back(element);
    element->bindSceneTree(mSceneTree);
    element->registerStatusObserver(
        std::bind(&InfiniteEngine::onElementStatusChanged, this, std::placeholders::_1));
}

bool InfiniteEngine::ifEngineStatusChanged() {
    return mLastStatus.sceneLineEnabled != mCurrentStatus.sceneLineEnabled;
}

bool InfiniteEngine::requestRenderFrame() {
    bool frameUpdated
        = !(mChangedElements.empty() && mAddedElements.empty() && !ifEngineStatusChanged());
    if (frameUpdated) {
        if (!mChangedElements.empty()) {
            // TODO
        } else if (!mAddedElements.empty()) {
            for (auto element : mAddedElements) {
                element->requestRender(mContext);
            }
        }
        if (mCurrentStatus.sceneLineEnabled) {
            auto rects = mSceneTree->ListTree();
            for (auto rect : rects) {
                mContext->strokeRect(rect.m_min[0], rect.m_min[1], rect.m_max[0] - rect.m_min[0],
                                     rect.m_max[1] - rect.m_min[1]);
            }
        }
        mContext->flush();
        std::unordered_map<int32_t, std::shared_ptr<Element>>().swap(mChangedElements);
        std::unordered_map<int32_t, std::shared_ptr<Element>>().swap(mAddedElementMap);
        std::vector<std::shared_ptr<Element>>().swap(mAddedElements);
        mLastStatus = mCurrentStatus;
    }
    return frameUpdated;
}

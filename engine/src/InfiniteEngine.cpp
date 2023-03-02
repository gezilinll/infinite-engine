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
            std::vector<SkRect> rectsToClear;
            std::vector<SkRect> rectsToDraw;
            for (auto element : mChangedElements) {
                rectsToClear.push_back(element.second->getRectToClear());
                rectsToDraw.push_back(element.second->getRectToDraw());
            }
            for (auto& rect : rectsToClear) {
                mContext->clearRect(rect.x(), rect.y(), rect.width(), rect.height());
                float min[2] = {rect.x(), rect.y()};
                float max[2] = {rect.x() + rect.width(), rect.y() + rect.height()};
                std::unordered_map<uint32_t, uint8_t> elementsToDraw;
                mSceneTree->Search(&min[0], &max[0], [&](const uint32_t& id) {
                    elementsToDraw.insert(std::make_pair(id, 1));
                    return true;
                });
                for (auto element : mElements) {
                    if (elementsToDraw.find(element->getID()) != elementsToDraw.end()
                        && element->getRectToDraw().intersect(rect)) {
                        element->requestRenderDirty(mContext, rect);
                    }
                }
            }
            for (auto& rect : rectsToDraw) {
                mContext->clearRect(rect.x(), rect.y(), rect.width(), rect.height());
                std::unordered_map<uint32_t, uint8_t> elementsToDraw;
                float min[2] = {rect.x(), rect.y()};
                float max[2] = {rect.x() + rect.width(), rect.y() + rect.height()};
                mSceneTree->Search(&min[0], &max[0], [&](const uint32_t& id) {
                    elementsToDraw.insert(std::make_pair(id, 1));
                    return true;
                });
                for (auto element : mElements) {
                    if (elementsToDraw.find(element->getID()) != elementsToDraw.end()) {
                        element->requestRenderDirty(mContext, rect);
                    }
                }
            }
        }
        if (!mAddedElements.empty()) {
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

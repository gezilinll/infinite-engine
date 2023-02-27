//
// Created by 林炳河 on 2023/2/27.
//

#include "FontManager.hpp"
#include "include/core/SkData.h"
#include "include/core/SkFontMgr.h"

void FontManager::loadFont(const void* data, size_t length, const FontInfo& info) {
    sk_sp<SkData> fontData = SkData::MakeFromMalloc(data, length);
    auto font = SkFontMgr::RefDefault()->makeFromData(fontData);
    std::string key = (info.style.empty() ? "normal" : info.style) + '|'
                      + (info.variant.empty() ? "normal" : info.variant) + '|'
                      + (info.weight.empty() ? "normal" : info.weight);
    std::string fam = info.family;
    auto familyIter = mFontCache.find(fam);
    if (familyIter == mFontCache.end()) {
        mFontCache[fam]["*"] = font;
    }
    mFontCache[fam][key] = font;
}

sk_sp<SkTypeface> FontManager::getTypeFaceFromCache(const FontInfo& info) {
    std::string key = (info.style.empty() ? "normal" : info.style) + '|'
                      + (info.variant.empty() ? "normal" : info.variant) + '|'
                      + (info.weight.empty() ? "normal" : info.weight);
    std::string fam = info.family;
    auto familyIter = mFontCache.find(fam);
    if (familyIter == mFontCache.end()) {
        return nullptr;
    }
    auto tfIter = familyIter->second.find(key);
    if (tfIter == familyIter->second.end()) {
        return familyIter->second["*"];
    }
    return tfIter->second;
}
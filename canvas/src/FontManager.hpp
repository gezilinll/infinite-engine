//
// Created by 林炳河 on 2023/2/27.
//

#ifndef INFINITEENGINE_FONTMANAGER_HPP
#define INFINITEENGINE_FONTMANAGER_HPP

#include <unordered_map>
#include "Models.hpp"
#include "include/core/SkFont.h"

class FontManager {
public:
    void loadFont(const void* data, size_t length, const FontInfo& info);

    sk_sp<SkTypeface> getTypeFaceFromCache(const FontInfo& info);

protected:
    std::unordered_map<std::string, std::unordered_map<std::string, sk_sp<SkTypeface>>> mFontCache;
};

#endif  // INFINITEENGINE_FONTMANAGER_HPP

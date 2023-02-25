//
// Created by 林炳河 on 2023/2/23.
//

#ifndef INFINITEENGINE_SKIAUTILS_HPP
#define INFINITEENGINE_SKIAUTILS_HPP
#include <string>
#include <unordered_map>
#include "SkiaModels.hpp"
#include "include/core/SkBlendMode.h"
#include "include/core/SkColor.h"

class SkiaUtils {
public:
    static Color parseColorString(std::string colorStr);

    static Color multiplyByAlpha(Color baseColor, float alpha);

    static SkBlendMode parseBlendModeString(std::string blendModeStr);

    static Font parseFontString(std::string fontStr);

private:
    static std::unordered_map<std::string, SkBlendMode> sBlendModeMap;
};

#endif  // INFINITEENGINE_SKIAUTILS_HPP

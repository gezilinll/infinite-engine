//
// Created by 林炳河 on 2023/2/23.
//

#ifndef INFINITEENGINE_SKIAUTILS_HPP
#define INFINITEENGINE_SKIAUTILS_HPP
#include <math.h>
#include <string>
#include <unordered_map>
#include "SkiaModels.hpp"
#include "include/core/SkBlendMode.h"
#include "include/core/SkColor.h"
#define PI acos(-1)

class SkiaUtils {
public:
    static SkColor parseColorString(std::string colorStr);

    static SkColor multiplyByAlpha(SkColor baseColor, float alpha);

    static SkBlendMode parseBlendModeString(std::string blendModeStr);

    static Font parseFontString(std::string fontStr);

    static SkScalar radiansToDegrees(SkScalar radians) { return (radians / PI) * 180; }

private:
    static void initColorMap();

    static void initBlendModeMap();

private:
    static std::unordered_map<std::string, SkColor> sColorMap;
    static std::unordered_map<std::string, SkBlendMode> sBlendModeMap;
};

#endif  // INFINITEENGINE_SKIAUTILS_HPP

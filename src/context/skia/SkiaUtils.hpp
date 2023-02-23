//
// Created by 林炳河 on 2023/2/23.
//

#ifndef INFINITEENGINE_SKIAUTILS_HPP
#define INFINITEENGINE_SKIAUTILS_HPP
#include <string>
#include "include/core/SkColor.h"

class SkiaUtils {
public:
    static SkColor parseColorString(std::string colorStr, float multiplyAlpha = 1.0);
};

#endif  // INFINITEENGINE_SKIAUTILS_HPP

//
// Created by 林炳河 on 2023/2/23.
//

#ifndef INFINITEENGINE_SKIAMODELS_HPP
#define INFINITEENGINE_SKIAMODELS_HPP
#include <string>
#include <vector>
#include "include/core/SkScalar.h"

enum class StrokeStyleType {
    Color
};

struct StrokeStyle {
    StrokeStyleType type = StrokeStyleType::Color;
    std::string color = "#000";
};

struct LineDash {
    std::vector<SkScalar> intervals;
    SkScalar phase = 0;
};

#endif //INFINITEENGINE_SKIAMODELS_HPP

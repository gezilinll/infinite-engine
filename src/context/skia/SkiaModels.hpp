//
// Created by 林炳河 on 2023/2/23.
//

#ifndef INFINITEENGINE_SKIAMODELS_HPP
#define INFINITEENGINE_SKIAMODELS_HPP
#include <string>
#include <vector>
#include "include/core/SkScalar.h"

// 255
struct Color {
    Color(float _r, float _g, float _b, float _a) {
        R = _r;
        G = _g;
        B = _b;
        A = _a;
    }

    float R = 0;
    float G = 0;
    float B = 0;
    float A = 0;
};

enum class StrokeStyleType { Color };

struct StrokeStyle {
    StrokeStyleType type = StrokeStyleType::Color;
    Color color = {0, 0, 0, 255};
};

struct LineDash {
    std::vector<SkScalar> intervals;
    SkScalar phase = 0;
};

#endif  // INFINITEENGINE_SKIAMODELS_HPP

//
// Created by 林炳河 on 2023/2/23.
//

#ifndef INFINITEENGINE_SKIAMODELS_HPP
#define INFINITEENGINE_SKIAMODELS_HPP
#include <string>
#include <vector>
#include "include/core/SkColor.h"
#include "include/core/SkScalar.h"

struct FillStrokeStyle {
    enum class StyleType { Color };

    StyleType type = StyleType::Color;
    SkColor color = SK_ColorBLACK;
};

#endif  // INFINITEENGINE_SKIAMODELS_HPP

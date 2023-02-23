//
// Created by 林炳河 on 2023/2/23.
//

#include "SkiaUtils.hpp"
#include <algorithm>
#include <string_view>

SkColor SkiaUtils::parseColorString(std::string colorStr, float multiplyAlpha) {
    std::transform(colorStr.begin(), colorStr.end(), colorStr.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    // See https://drafts.csswg.org/css-color/#typedef-hex-color
    if (colorStr.rfind('#', 0) == 0) {
        int r, g, b, a = 255;
        switch (colorStr.length()) {
            case 9:  // 8 hex chars #RRGGBBAA
                a = std::stoi(colorStr.substr(7, 9), 0, 16);
            case 7:  // 6 hex chars #RRGGBB
                r = std::stoi(colorStr.substr(1, 3), 0, 16);
                g = std::stoi(colorStr.substr(3, 5), 0, 16);
                b = std::stoi(colorStr.substr(5, 7), 0, 16);
                break;
            case 5:  // 4 hex chars #RGBA
                // multiplying by 17 is the same effect as
                // appending another character of the same value
                // e.g. e => ee == 14 => 238
                a = std::stoi(colorStr.substr(4, 5), 0, 16) * 17;
            case 4:  // 6 hex chars #RGB
                r = std::stoi(colorStr.substr(1, 2), 0, 16) * 17;
                g = std::stoi(colorStr.substr(2, 3), 0, 16) * 17;
                b = std::stoi(colorStr.substr(3, 4), 0, 16) * 17;
                break;
        }
        return SkColorSetARGB(std::min(255, std::max(0, static_cast<int>(a * multiplyAlpha))), r, g,
                              b);
    } else if (colorStr.rfind('rgba', 0) == 0) {
        // TODO
    } else if (colorStr.rfind('rgb', 0) == 0) {
        // TODO
    } else if (colorStr.rfind('gray(', 0) == 0) {
        // TODO
    } else if (colorStr.rfind('hsl', 0) == 0) {
        // TODO
    } else {
        // TODO
    }
    printf("unrecognized color:%s\n", colorStr.data());
    return SK_ColorBLACK;
}
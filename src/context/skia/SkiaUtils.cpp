//
// Created by 林炳河 on 2023/2/23.
//

#include "SkiaUtils.hpp"
#include <algorithm>
#include <sstream>

std::unordered_map<std::string, SkBlendMode> SkiaUtils::sBlendModeMap;

std::vector<std::string> splitStr(const std::string& s, char delim) {
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (getline(ss, item, delim)) {
        if (!item.empty()) {
            elems.push_back(item);
        }
    }
    return elems;
}

Color SkiaUtils::parseColorString(std::string colorStr) {
    std::transform(colorStr.begin(), colorStr.end(), colorStr.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    // See https://drafts.csswg.org/css-color/#typedef-hex-color
    if (colorStr[0] == '#') {
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
        return Color(r, g, b, a);
    } else if (colorStr.rfind("rgba", 0) == 0) {
        colorStr = colorStr.substr(5);
        auto splitNums = splitStr(colorStr, ',');
        return Color{static_cast<float>(std::stoi(splitNums[0], 0)),
                     static_cast<float>(std::stoi(splitNums[1], 0)),
                     static_cast<float>(std::stoi(splitNums[2], 0)),
                     static_cast<float>(std::stoi(splitNums[3], 0))};
    } else if (colorStr.rfind("rgb", 0) == 0) {
        colorStr = colorStr.substr(4);
        auto splitNums = splitStr(colorStr, ',');
        return Color{static_cast<float>(std::stoi(splitNums[0], 0)),
                     static_cast<float>(std::stoi(splitNums[1], 0)),
                     static_cast<float>(std::stoi(splitNums[2], 0)), 255.f};
    } else if (colorStr.rfind("gray(", 0) == 0) {
        // TODO
    } else if (colorStr.rfind("hsl", 0) == 0) {
        // TODO
    } else {
        // TODO
    }
    printf("unrecognized color:%s\n", colorStr.data());
    return Color(0, 0, 0, 255);
}

Color SkiaUtils::multiplyByAlpha(Color baseColor, float alpha) {
    return Color{baseColor.R, baseColor.G, baseColor.B,
                 std::max(0.f, std::min(baseColor.A * alpha, 255.0f))};
}

SkBlendMode SkiaUtils::parseBlendModeString(std::string blendModeStr) {
    if (sBlendModeMap.empty()) {
        // composite-mode
        sBlendModeMap["source-over"] = SkBlendMode::kSrcOver;
        sBlendModeMap["destination-over"] = SkBlendMode::kDstOver;
        sBlendModeMap["copy"] = SkBlendMode::kSrc;
        sBlendModeMap["destination"] = SkBlendMode::kDst;
        sBlendModeMap["clear"] = SkBlendMode::kClear;
        sBlendModeMap["source-in"] = SkBlendMode::kSrcIn;
        sBlendModeMap["destination-in"] = SkBlendMode::kDstIn;
        sBlendModeMap["source-out"] = SkBlendMode::kSrcOut;
        sBlendModeMap["destination-out"] = SkBlendMode::kDstOut;
        sBlendModeMap["source-atop"] = SkBlendMode::kSrcATop;
        sBlendModeMap["destination-atop"] = SkBlendMode::kDstATop;
        sBlendModeMap["xor"] = SkBlendMode::kXor;
        sBlendModeMap["lighter"] = SkBlendMode::kPlus;
        sBlendModeMap["plus-lighter"] = SkBlendMode::kPlus;
        // TODO:plus-darker
        //  blend-mode
        sBlendModeMap["multiply"] = SkBlendMode::kMultiply;
        sBlendModeMap["screen"] = SkBlendMode::kScreen;
        sBlendModeMap["overlay"] = SkBlendMode::kOverlay;
        sBlendModeMap["darken"] = SkBlendMode::kDarken;
        sBlendModeMap["lighten"] = SkBlendMode::kLighten;
        sBlendModeMap["color-dodge"] = SkBlendMode::kColorDodge;
        sBlendModeMap["color-burn"] = SkBlendMode::kColorBurn;
        sBlendModeMap["hard-light"] = SkBlendMode::kHardLight;
        sBlendModeMap["soft-light"] = SkBlendMode::kSoftLight;
        sBlendModeMap["difference"] = SkBlendMode::kDifference;
        sBlendModeMap["exclusion"] = SkBlendMode::kExclusion;
        sBlendModeMap["hue"] = SkBlendMode::kHue;
        sBlendModeMap["saturation"] = SkBlendMode::kSaturation;
        sBlendModeMap["color"] = SkBlendMode::kColor;
        sBlendModeMap["luminosity"] = SkBlendMode::kLuminosity;
    }
    auto it = sBlendModeMap.find(blendModeStr);
    if (it == sBlendModeMap.end()) {
        throw blendModeStr + " is not supported";
    }
    return it->second;
}
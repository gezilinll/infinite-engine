//
// Created by 林炳河 on 2023/2/23.
//

#include "SkiaUtils.hpp"
#include <algorithm>
#include <regex>
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

Font SkiaUtils::parseFontString(std::string fontStr) {
    static std::regex regex(
        reinterpret_cast<const char*>('(italic|oblique|normal|)\\s*' +               // style
                                      '(small-caps|normal|)\\s*' +                   // variant
                                      '(bold|bolder|lighter|[1-9]00|normal|)\\s*' +  // weight
                                      '([\\d\\.]+)' +                                // size
                                      "(px|pt|pc|in|cm|mm|%|em|ex|ch|rem|q)" +       // unit
                                      // line-height is ignored here, as per the spec
                                      '(.+)'));  // family
    static int defaultHeight = 16;
    std::cmatch cm;
    std::regex_match(fontStr.data(), cm, regex);
    if (cm.empty()) {
        throw "Invalid font string " + fontStr;
    }
    float size = std::stof(cm[4]);
    int sizePx = defaultHeight;
    std::string unit = cm[5];
    if (unit == "em" || unit == "rem") {
        sizePx = size * defaultHeight;
    } else if (unit == "pt") {
        sizePx = size * 4 / 3;
    } else if (unit == "px") {
        size = size;
    } else if (unit == "pc") {
        sizePx = size * defaultHeight;
    } else if (unit == "in") {
        sizePx = size * 96;
    } else if (unit == "cm") {
        sizePx = size * 96.0 / 2.54;
    } else if (unit == "mm") {
        sizePx = size * (96.0 / 25.4);
    } else if (unit == "q") {
        sizePx = size * (96.0 / 25.4 / 4);
    } else if (unit == "%") {
        sizePx = size * (defaultHeight / 75);
    }
    Font result;
    result.style = cm[1];
    result.variant = cm[2];
    result.weight = cm[3];
    result.sizePx = sizePx;
    result.family = cm[6];
    result.family.erase(remove(result.family.begin(), result.family.end(), ' '),
                        result.family.end());

    return result;
}
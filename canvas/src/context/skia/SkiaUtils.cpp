//
// Created by 林炳河 on 2023/2/23.
//

#include "SkiaUtils.hpp"
#include <algorithm>
#include <regex>
#include <sstream>

std::unordered_map<std::string, SkColor> SkiaUtils::sColorMap;
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

SkColor SkiaUtils::parseColorString(std::string colorStr) {
    std::transform(colorStr.begin(), colorStr.end(), colorStr.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    // See https://drafts.csswg.org/css-color/#typedef-hex-color
    if (colorStr[0] == '#') {
        int r, g, b, a = 255;
        switch (colorStr.length()) {
            case 9:  // 8 hex chars #RRGGBBAA
                a = std::stoi(colorStr.substr(7, 2), 0, 16);
            case 7:  // 6 hex chars #RRGGBB
                r = std::stoi(colorStr.substr(1, 2), 0, 16);
                g = std::stoi(colorStr.substr(3, 2), 0, 16);
                b = std::stoi(colorStr.substr(5, 2), 0, 16);
                break;
            case 5:  // 4 hex chars #RGBA
                // multiplying by 17 is the same effect as
                // appending another character of the same value
                // e.g. e => ee == 14 => 238
                a = std::stoi(colorStr.substr(4, 1), 0, 16) * 17;
            case 4:  // 6 hex chars #RGB
                r = std::stoi(colorStr.substr(1, 1), 0, 16) * 17;
                g = std::stoi(colorStr.substr(2, 1), 0, 16) * 17;
                b = std::stoi(colorStr.substr(3, 1), 0, 16) * 17;
                break;
        }
        return SkColorSetARGB(a, r, g, b);
    } else if (colorStr.rfind("rgba", 0) == 0) {
        colorStr = colorStr.substr(5);
        auto splitNums = splitStr(colorStr, ',');
        return SkColorSetARGB(static_cast<float>(std::stoi(splitNums[3], 0)),
                              static_cast<float>(std::stoi(splitNums[0], 0)),
                              static_cast<float>(std::stoi(splitNums[1], 0)),
                              static_cast<float>(std::stoi(splitNums[2], 0)));
    } else if (colorStr.rfind("rgb", 0) == 0) {
        colorStr = colorStr.substr(4);
        auto splitNums = splitStr(colorStr, ',');
        return SkColorSetARGB(255.f, static_cast<float>(std::stoi(splitNums[0], 0)),
                              static_cast<float>(std::stoi(splitNums[1], 0)),
                              static_cast<float>(std::stoi(splitNums[2], 0)));
    } else if (colorStr.rfind("gray(", 0) == 0) {
        // TODO
    } else if (colorStr.rfind("hsl", 0) == 0) {
        // TODO
    } else {
        if (sColorMap.empty()) {
            initColorMap();
        }
        auto it = sColorMap.find(colorStr);
        if (it != sColorMap.end()) {
            return it->second;
        }
    }
    printf("unrecognized color:%s\n", colorStr.data());
    return SK_ColorBLACK;
}

SkColor SkiaUtils::multiplyByAlpha(SkColor baseColor, float alpha) {
    return SkColorSetARGB(std::max(0.f, std::min(SkColorGetA(baseColor) * alpha, 255.0f)),
                          SkColorGetR(baseColor), SkColorGetG(baseColor), SkColorGetB(baseColor));
}

SkBlendMode SkiaUtils::parseBlendModeString(std::string blendModeStr) {
    if (sBlendModeMap.empty()) {
        initBlendModeMap();
    }
    auto it = sBlendModeMap.find(blendModeStr);
    if (it == sBlendModeMap.end()) {
        throw std::string(blendModeStr + " is not supported");
    }
    return it->second;
}

FontInfo SkiaUtils::parseFontString(std::string fontStr) {
    static std::string regexStr
        = std::string("(italic|oblique|normal|)\\s*")    // style
          + "(small-caps|normal|)\\s*"                   // variant
          + "(bold|bolder|lighter|[1-9]00|normal|)\\s*"  // weight
          + "([\\d\\.]+)"                                // size
          + "(px|pt|pc|in|cm|mm|%|em|ex|ch|rem|q)"       // unit
                                                    // line-height is ignored here, as per the spec
          + "(.+)";  // family
    static std::regex regex(regexStr);
    static int defaultHeight = 16;
    std::cmatch cm;
    std::regex_match(fontStr.data(), cm, regex);
    if (cm.empty()) {
        throw std::string("Invalid font string " + fontStr);
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
    FontInfo result;
    result.style = cm[1];
    result.variant = cm[2];
    result.weight = cm[3];
    result.sizePx = sizePx;
    result.family = cm[6];
    result.family.erase(remove(result.family.begin(), result.family.end(), ' '),
                        result.family.end());

    return result;
}

void SkiaUtils::initColorMap() {
    std::function parseValue = [&](float r, float g, float b, float a) -> SkColor {
        return SkColorSetARGB(static_cast<int>(a * 255), static_cast<int>(r * 255),
                              static_cast<int>(g * 255), static_cast<int>(b * 255));
    };

    sColorMap["aliceblue"] = parseValue(0.941, 0.973, 1.000, 1.000),
    sColorMap["antiquewhite"] = parseValue(0.980, 0.922, 0.843, 1.000),
    sColorMap["aqua"] = parseValue(0.000, 1.000, 1.000, 1.000),
    sColorMap["aquamarine"] = parseValue(0.498, 1.000, 0.831, 1.000),
    sColorMap["azure"] = parseValue(0.941, 1.000, 1.000, 1.000),
    sColorMap["beige"] = parseValue(0.961, 0.961, 0.863, 1.000),
    sColorMap["bisque"] = parseValue(1.000, 0.894, 0.769, 1.000),
    sColorMap["black"] = parseValue(0.000, 0.000, 0.000, 1.000),
    sColorMap["blanchedalmond"] = parseValue(1.000, 0.922, 0.804, 1.000),
    sColorMap["blue"] = parseValue(0.000, 0.000, 1.000, 1.000),
    sColorMap["blueviolet"] = parseValue(0.541, 0.169, 0.886, 1.000),
    sColorMap["brown"] = parseValue(0.647, 0.165, 0.165, 1.000),
    sColorMap["burlywood"] = parseValue(0.871, 0.722, 0.529, 1.000),
    sColorMap["cadetblue"] = parseValue(0.373, 0.620, 0.627, 1.000),
    sColorMap["chartreuse"] = parseValue(0.498, 1.000, 0.000, 1.000),
    sColorMap["chocolate"] = parseValue(0.824, 0.412, 0.118, 1.000),
    sColorMap["coral"] = parseValue(1.000, 0.498, 0.314, 1.000),
    sColorMap["cornflowerblue"] = parseValue(0.392, 0.584, 0.929, 1.000),
    sColorMap["cornsilk"] = parseValue(1.000, 0.973, 0.863, 1.000),
    sColorMap["crimson"] = parseValue(0.863, 0.078, 0.235, 1.000),
    sColorMap["cyan"] = parseValue(0.000, 1.000, 1.000, 1.000),
    sColorMap["darkblue"] = parseValue(0.000, 0.000, 0.545, 1.000),
    sColorMap["darkcyan"] = parseValue(0.000, 0.545, 0.545, 1.000),
    sColorMap["darkgoldenrod"] = parseValue(0.722, 0.525, 0.043, 1.000),
    sColorMap["darkgray"] = parseValue(0.663, 0.663, 0.663, 1.000),
    sColorMap["darkgreen"] = parseValue(0.000, 0.392, 0.000, 1.000),
    sColorMap["darkgrey"] = parseValue(0.663, 0.663, 0.663, 1.000),
    sColorMap["darkkhaki"] = parseValue(0.741, 0.718, 0.420, 1.000),
    sColorMap["darkmagenta"] = parseValue(0.545, 0.000, 0.545, 1.000),
    sColorMap["darkolivegreen"] = parseValue(0.333, 0.420, 0.184, 1.000),
    sColorMap["darkorange"] = parseValue(1.000, 0.549, 0.000, 1.000),
    sColorMap["darkorchid"] = parseValue(0.600, 0.196, 0.800, 1.000),
    sColorMap["darkred"] = parseValue(0.545, 0.000, 0.000, 1.000),
    sColorMap["darksalmon"] = parseValue(0.914, 0.588, 0.478, 1.000),
    sColorMap["darkseagreen"] = parseValue(0.561, 0.737, 0.561, 1.000),
    sColorMap["darkslateblue"] = parseValue(0.282, 0.239, 0.545, 1.000),
    sColorMap["darkslategray"] = parseValue(0.184, 0.310, 0.310, 1.000),
    sColorMap["darkslategrey"] = parseValue(0.184, 0.310, 0.310, 1.000),
    sColorMap["darkturquoise"] = parseValue(0.000, 0.808, 0.820, 1.000),
    sColorMap["darkviolet"] = parseValue(0.580, 0.000, 0.827, 1.000),
    sColorMap["deeppink"] = parseValue(1.000, 0.078, 0.576, 1.000),
    sColorMap["deepskyblue"] = parseValue(0.000, 0.749, 1.000, 1.000),
    sColorMap["dimgray"] = parseValue(0.412, 0.412, 0.412, 1.000),
    sColorMap["dimgrey"] = parseValue(0.412, 0.412, 0.412, 1.000),
    sColorMap["dodgerblue"] = parseValue(0.118, 0.565, 1.000, 1.000),
    sColorMap["firebrick"] = parseValue(0.698, 0.133, 0.133, 1.000),
    sColorMap["floralwhite"] = parseValue(1.000, 0.980, 0.941, 1.000),
    sColorMap["forestgreen"] = parseValue(0.133, 0.545, 0.133, 1.000),
    sColorMap["fuchsia"] = parseValue(1.000, 0.000, 1.000, 1.000),
    sColorMap["gainsboro"] = parseValue(0.863, 0.863, 0.863, 1.000),
    sColorMap["ghostwhite"] = parseValue(0.973, 0.973, 1.000, 1.000),
    sColorMap["gold"] = parseValue(1.000, 0.843, 0.000, 1.000),
    sColorMap["goldenrod"] = parseValue(0.855, 0.647, 0.125, 1.000),
    sColorMap["gray"] = parseValue(0.502, 0.502, 0.502, 1.000),
    sColorMap["green"] = parseValue(0.000, 0.502, 0.000, 1.000),
    sColorMap["greenyellow"] = parseValue(0.678, 1.000, 0.184, 1.000),
    sColorMap["grey"] = parseValue(0.502, 0.502, 0.502, 1.000),
    sColorMap["honeydew"] = parseValue(0.941, 1.000, 0.941, 1.000),
    sColorMap["hotpink"] = parseValue(1.000, 0.412, 0.706, 1.000),
    sColorMap["indianred"] = parseValue(0.804, 0.361, 0.361, 1.000),
    sColorMap["indigo"] = parseValue(0.294, 0.000, 0.510, 1.000),
    sColorMap["ivory"] = parseValue(1.000, 1.000, 0.941, 1.000),
    sColorMap["khaki"] = parseValue(0.941, 0.902, 0.549, 1.000),
    sColorMap["lavender"] = parseValue(0.902, 0.902, 0.980, 1.000),
    sColorMap["lavenderblush"] = parseValue(1.000, 0.941, 0.961, 1.000),
    sColorMap["lawngreen"] = parseValue(0.486, 0.988, 0.000, 1.000),
    sColorMap["lemonchiffon"] = parseValue(1.000, 0.980, 0.804, 1.000),
    sColorMap["lightblue"] = parseValue(0.678, 0.847, 0.902, 1.000),
    sColorMap["lightcoral"] = parseValue(0.941, 0.502, 0.502, 1.000),
    sColorMap["lightcyan"] = parseValue(0.878, 1.000, 1.000, 1.000),
    sColorMap["lightgoldenrodyellow"] = parseValue(0.980, 0.980, 0.824, 1.000),
    sColorMap["lightgray"] = parseValue(0.827, 0.827, 0.827, 1.000),
    sColorMap["lightgreen"] = parseValue(0.565, 0.933, 0.565, 1.000),
    sColorMap["lightgrey"] = parseValue(0.827, 0.827, 0.827, 1.000),
    sColorMap["lightpink"] = parseValue(1.000, 0.714, 0.757, 1.000),
    sColorMap["lightsalmon"] = parseValue(1.000, 0.627, 0.478, 1.000),
    sColorMap["lightseagreen"] = parseValue(0.125, 0.698, 0.667, 1.000),
    sColorMap["lightskyblue"] = parseValue(0.529, 0.808, 0.980, 1.000),
    sColorMap["lightslategray"] = parseValue(0.467, 0.533, 0.600, 1.000),
    sColorMap["lightslategrey"] = parseValue(0.467, 0.533, 0.600, 1.000),
    sColorMap["lightsteelblue"] = parseValue(0.690, 0.769, 0.871, 1.000),
    sColorMap["lightyellow"] = parseValue(1.000, 1.000, 0.878, 1.000),
    sColorMap["lime"] = parseValue(0.000, 1.000, 0.000, 1.000),
    sColorMap["limegreen"] = parseValue(0.196, 0.804, 0.196, 1.000),
    sColorMap["linen"] = parseValue(0.980, 0.941, 0.902, 1.000),
    sColorMap["magenta"] = parseValue(1.000, 0.000, 1.000, 1.000),
    sColorMap["maroon"] = parseValue(0.502, 0.000, 0.000, 1.000),
    sColorMap["mediumaquamarine"] = parseValue(0.400, 0.804, 0.667, 1.000),
    sColorMap["mediumblue"] = parseValue(0.000, 0.000, 0.804, 1.000),
    sColorMap["mediumorchid"] = parseValue(0.729, 0.333, 0.827, 1.000),
    sColorMap["mediumpurple"] = parseValue(0.576, 0.439, 0.859, 1.000),
    sColorMap["mediumseagreen"] = parseValue(0.235, 0.702, 0.443, 1.000),
    sColorMap["mediumslateblue"] = parseValue(0.482, 0.408, 0.933, 1.000),
    sColorMap["mediumspringgreen"] = parseValue(0.000, 0.980, 0.604, 1.000),
    sColorMap["mediumturquoise"] = parseValue(0.282, 0.820, 0.800, 1.000),
    sColorMap["mediumvioletred"] = parseValue(0.780, 0.082, 0.522, 1.000),
    sColorMap["midnightblue"] = parseValue(0.098, 0.098, 0.439, 1.000),
    sColorMap["mintcream"] = parseValue(0.961, 1.000, 0.980, 1.000),
    sColorMap["mistyrose"] = parseValue(1.000, 0.894, 0.882, 1.000),
    sColorMap["moccasin"] = parseValue(1.000, 0.894, 0.710, 1.000),
    sColorMap["navajowhite"] = parseValue(1.000, 0.871, 0.678, 1.000),
    sColorMap["navy"] = parseValue(0.000, 0.000, 0.502, 1.000),
    sColorMap["oldlace"] = parseValue(0.992, 0.961, 0.902, 1.000),
    sColorMap["olive"] = parseValue(0.502, 0.502, 0.000, 1.000),
    sColorMap["olivedrab"] = parseValue(0.420, 0.557, 0.137, 1.000),
    sColorMap["orange"] = parseValue(1.000, 0.647, 0.000, 1.000),
    sColorMap["orangered"] = parseValue(1.000, 0.271, 0.000, 1.000),
    sColorMap["orchid"] = parseValue(0.855, 0.439, 0.839, 1.000),
    sColorMap["palegoldenrod"] = parseValue(0.933, 0.910, 0.667, 1.000),
    sColorMap["palegreen"] = parseValue(0.596, 0.984, 0.596, 1.000),
    sColorMap["paleturquoise"] = parseValue(0.686, 0.933, 0.933, 1.000),
    sColorMap["palevioletred"] = parseValue(0.859, 0.439, 0.576, 1.000),
    sColorMap["papayawhip"] = parseValue(1.000, 0.937, 0.835, 1.000),
    sColorMap["peachpuff"] = parseValue(1.000, 0.855, 0.725, 1.000),
    sColorMap["peru"] = parseValue(0.804, 0.522, 0.247, 1.000),
    sColorMap["pink"] = parseValue(1.000, 0.753, 0.796, 1.000),
    sColorMap["plum"] = parseValue(0.867, 0.627, 0.867, 1.000),
    sColorMap["powderblue"] = parseValue(0.690, 0.878, 0.902, 1.000),
    sColorMap["purple"] = parseValue(0.502, 0.000, 0.502, 1.000),
    sColorMap["rebeccapurple"] = parseValue(0.400, 0.200, 0.600, 1.000),
    sColorMap["red"] = parseValue(1.000, 0.000, 0.000, 1.000),
    sColorMap["rosybrown"] = parseValue(0.737, 0.561, 0.561, 1.000),
    sColorMap["royalblue"] = parseValue(0.255, 0.412, 0.882, 1.000),
    sColorMap["saddlebrown"] = parseValue(0.545, 0.271, 0.075, 1.000),
    sColorMap["salmon"] = parseValue(0.980, 0.502, 0.447, 1.000),
    sColorMap["sandybrown"] = parseValue(0.957, 0.643, 0.376, 1.000),
    sColorMap["seagreen"] = parseValue(0.180, 0.545, 0.341, 1.000),
    sColorMap["seashell"] = parseValue(1.000, 0.961, 0.933, 1.000),
    sColorMap["sienna"] = parseValue(0.627, 0.322, 0.176, 1.000),
    sColorMap["silver"] = parseValue(0.753, 0.753, 0.753, 1.000),
    sColorMap["skyblue"] = parseValue(0.529, 0.808, 0.922, 1.000),
    sColorMap["slateblue"] = parseValue(0.416, 0.353, 0.804, 1.000),
    sColorMap["slategray"] = parseValue(0.439, 0.502, 0.565, 1.000),
    sColorMap["slategrey"] = parseValue(0.439, 0.502, 0.565, 1.000),
    sColorMap["snow"] = parseValue(1.000, 0.980, 0.980, 1.000),
    sColorMap["springgreen"] = parseValue(0.000, 1.000, 0.498, 1.000),
    sColorMap["steelblue"] = parseValue(0.275, 0.510, 0.706, 1.000),
    sColorMap["tan"] = parseValue(0.824, 0.706, 0.549, 1.000),
    sColorMap["teal"] = parseValue(0.000, 0.502, 0.502, 1.000),
    sColorMap["thistle"] = parseValue(0.847, 0.749, 0.847, 1.000),
    sColorMap["tomato"] = parseValue(1.000, 0.388, 0.278, 1.000),
    sColorMap["transparent"] = parseValue(0.000, 0.000, 0.000, 0.000),
    sColorMap["turquoise"] = parseValue(0.251, 0.878, 0.816, 1.000),
    sColorMap["violet"] = parseValue(0.933, 0.510, 0.933, 1.000),
    sColorMap["wheat"] = parseValue(0.961, 0.871, 0.702, 1.000),
    sColorMap["white"] = parseValue(1.000, 1.000, 1.000, 1.000),
    sColorMap["whitesmoke"] = parseValue(0.961, 0.961, 0.961, 1.000),
    sColorMap["yellow"] = parseValue(1.000, 1.000, 0.000, 1.000),
    sColorMap["yellowgreen"] = parseValue(0.604, 0.804, 0.196, 1.000);
}

void SkiaUtils::initBlendModeMap() {
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
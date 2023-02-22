//
// Created by 林炳河 on 2023/2/22.
//

#ifndef INFINITEENGINE_MACROS_HPP
#define INFINITEENGINE_MACROS_HPP

// Web
#if defined(__EMSCRIPTEN__)
#define IS_WEB 1
#else
#define IS_WEB 0
#endif

#endif //INFINITEENGINE_MACROS_HPP

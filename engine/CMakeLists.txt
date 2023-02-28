cmake_minimum_required(VERSION 3.5)
project(InfiniteEngine VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DSK_GL)

include_directories(../3rdparty/skia/include)
include_directories(../3rdparty/skia/include/include/core)
include_directories(../canvas/src)
include_directories(../canvas/src/context)
include_directories(../canvas/src/context/skia)
aux_source_directory(../canvas/src DIR_SOURCE_CANVAS)
aux_source_directory(../canvas/src/context DIR_CONTEXT_CANVAS)
aux_source_directory(../canvas/src/context/skia DIR_SKIA_CANVAS)
include_directories(src)
include_directories(src/element)
aux_source_directory(src DIR_SOURCE)
aux_source_directory(src/element DIR_SOURCE_ELEMENT)

add_library(skia_lib STATIC IMPORTED)
set_target_properties(
    skia_lib
    PROPERTIES IMPORTED_LOCATION
        ../3rdparty/skia/mac_intel/libskia.a
)

add_library(
    InfiniteEngine
    SHARED
    ${DIR_SOURCE}
    ${DIR_SOURCE_CANVAS}
    ${DIR_CONTEXT_CANVAS}
    ${DIR_SKIA_CANVAS}
    ${DIR_SOURCE_ELEMENT}
)
target_link_libraries(
    InfiniteEngine
    skia_lib
)
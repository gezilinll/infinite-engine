cmake_minimum_required(VERSION 3.5)
project(InfiniteEngine VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DSK_GL)

include_directories(../3rdparty/skia/include)
include_directories(../3rdparty/skia/include/include/core)
include_directories(src)
include_directories(src/context)
include_directories(src/context/skia)
aux_source_directory(src DIR_SOURCE)
aux_source_directory(src/context DIR_CONTEXT)
aux_source_directory(src/context/skia DIR_SKIA)


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
    ${DIR_CONTEXT}
    ${DIR_SKIA}
)
target_link_libraries(
    InfiniteEngine
    skia_lib
)
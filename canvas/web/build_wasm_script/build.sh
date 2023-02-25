#!/bin/bash

set -e

# 编译
export OPTIMIZE="-O0 -DNDEBUG -DSK_GL -Wl,--no-check-features"
export LDFLAGS="${OPTIMIZE}"
export CFLAGS="${OPTIMIZE}"
export CPPFLAGS="${OPTIMIZE}"

NAME="InfiniteCanvas"
work_path='build-wasm'
rm -fr ${work_path}
mkdir -p ${work_path}
cd ./${work_path}

echo "============================================="
echo "Compiling InfiniteCanvas Module"
echo "============================================="
(
  emcc \
  --bind \
  ${OPTIMIZE} \
  -flto \
  -fexceptions \
  -s ASSERTIONS=0 \
  -s WASM=1 \
  -s ENVIRONMENT=web \
  -s ALLOW_MEMORY_GROWTH=1 \
  -s OFFSCREEN_FRAMEBUFFER=1 \
  -s OFFSCREENCANVAS_SUPPORT=1 \
  -s EXTRA_EXPORTED_RUNTIME_METHODS='["GL"]' \
  -s SAFE_HEAP=0 \
  -s MODULARIZE=1 \
  -s EXPORT_ES6=1 \
  -s FULL_ES3=1 \
  -s EXPORT_NAME=${NAME} \
  -s USE_ES6_IMPORT_META=0 \
  -I ../src/bridge/ \
  -I ../../src/ \
  -I ../../src/context \
  -I ../../src/context/skia \
  -I ../../3rdparty/skia/include \
  -I ../../3rdparty/skia/include/include/core \
  -g \
  -std=c++17 \
  ../src/bridge/*.cpp \
  ../../src/*.cpp \
  /Users/linbinghe/Github/skia-builder/skia/src/fonts/NotoMono-Regular.ttf.cpp \
  ../../src/context/skia/*.cpp \
  ../../3rdparty/skia/web/libskia.a \
  -o ${NAME}.js
)

cd ..

cp build-wasm/${NAME}.wasm src/wasm
cp build-wasm/${NAME}.js src/wasm

cp build-wasm/${NAME}.wasm demo

echo "============================================="
echo "InfiniteCanvas module Compiled"
echo "============================================="
#!/bin/bash

source /c/Users/mday/emsdk/emsdk_env.sh

rm -r build
mkdir build
rm -r dist
mkdir dist

em++ --std=c++11 --bind -s WASM=1 -s MODULARIZE=1 -s ENVIRONMENT=web -Iinclude -Ilib $(find . -type f -name "*.cpp") -o build/loop.js

cp build/loop.wasm dist/loop.wasm
cp build/loop.js dist/loop_mod.js
cp js/main.js dist/main.js
cp js/wasm_loader.js dist/wasm_loader.js

echo "All done!"
sleep 100
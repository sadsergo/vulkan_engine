#!/bin/bash

bash compile_shaders.sh

cd build
make -j20
cd ..

./build/engine
#!/bin/bash

glslc shaders/simple_shader.vert -o shaders/simple_shader.vert.spv
glslc shaders/simple_shader.frag -o shaders/simple_shader.frag.spv

cd build
cmake ..
make -j20
cd ..

./build/engine
cmake_minimum_required(VERSION 3.13)
project(engine)

set(CMAKE_CXX_STANDARD 23)

find_package(SDL2 REQUIRED)
include_directories(prj ${SDL2_INCLUDE_DIRS})

add_executable(engine src/main.cpp vk-bootstrap/VkBootstrap.cpp)
target_link_libraries(engine vulkan)
target_include_directories(engine PUBLIC vk-bootstrap)

find_package(fmt)
target_link_libraries(engine fmt::fmt ${SDL2_LIBRARIES})
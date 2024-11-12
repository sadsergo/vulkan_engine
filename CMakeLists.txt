cmake_minimum_required(VERSION 3.13)
project(engine)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
find_package(glfw3 REQUIRED)
include_directories(prj ${SDL2_INCLUDE_DIRS})

add_executable(engine 
                    src/main.cpp 
                    
                    src/glfw_extensions/window.cpp 
                    src/glfw_extensions/app.cpp 
                    src/glfw_extensions/camera.cpp
                    src/glfw_extensions/keyboard.cpp
                    
                    src/graphics_pipeline/pipeline.cpp 
                    
                    src/renderer/renderer.cpp
                    src/renderer/render_system.cpp

                    src/vulkan_code/lve_device.cpp 
                    src/vulkan_code/lve_swap_chain.cpp

                    src/model/model.cpp
                    src/game_object/game_object.cpp

                    vk-bootstrap/VkBootstrap.cpp)
target_link_libraries(engine vulkan glfw)
target_include_directories(engine PUBLIC vk-bootstrap)

find_package(fmt)
target_link_libraries(engine fmt::fmt ${SDL2_LIBRARIES})
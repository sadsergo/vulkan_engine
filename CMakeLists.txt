cmake_minimum_required(VERSION 3.13)
project(engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (UNIX)
find_package(glfw3 REQUIRED)
else()
find_package(Vulkan REQUIRED)
endif()

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

            src/uniform_buffer/buffer.cpp

            vk-bootstrap/VkBootstrap.cpp
)

if (UNIX)
target_link_libraries(engine vulkan glfw)
else()

#   Set GLFW paths
set(GLFW_INCLUDE_DIR "E:/vulkan/glfw-3.4.bin.WIN64/include")
set(GLFW_LIBRARY_DIR "E:/vulkan/glfw-3.4.bin.WIN64/lib-vc2022")

#   Include GlFW headers
include_directories(${GLFW_INCLUDE_DIR})

#   Link GLFW libraries
target_link_libraries(engine PRIVATE ${GLFW_LIBRARY_DIR}/glfw3.lib Vulkan::Vulkan)
endif()

target_include_directories(engine PUBLIC vk-bootstrap)
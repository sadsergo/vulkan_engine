#pragma once

#include "window.hpp"
#include "../graphics_pipeline/pipeline.hpp"
#include "../vulkan_code/lve_device.hpp"

namespace lve
{
    class App
    {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        void run();
    private:
        LveWindow window {WIDTH, HEIGHT, "game engine window"};
        LveDevice device { window };
        Pipeline pipeline {device, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", Pipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
    };
};
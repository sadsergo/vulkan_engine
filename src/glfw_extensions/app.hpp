#pragma once

#include "window.hpp"
#include "../graphics_pipeline/pipeline.hpp"

namespace lve
{
    class App
    {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        void run();
    private:
        LveWindow lveWindow {WIDTH, HEIGHT, "game engine window"};
        Pipeline pipeline {"shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv"};
    };
};
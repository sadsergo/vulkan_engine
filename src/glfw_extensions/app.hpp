#pragma once

#include "window.hpp"
#include "../graphics_pipeline/pipeline.hpp"
#include "../vulkan_code/lve_device.hpp"
#include "../vulkan_code/lve_swap_chain.hpp"
#include "../model/model.hpp"

#include <memory>
#include <vector>

namespace lve
{
    class App
    {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        App();
        ~App();

        App(const App &) = delete;
        App &operator=(const App &) = delete;

        void run();
    private:
        void loadModels();
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void drawFrame();

        LveWindow window {WIDTH, HEIGHT, "game engine window"};
        LveDevice device { window };
        LveSwapChain swapchain { device, window.getExtent() };
        std::unique_ptr<Pipeline> pipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
        std::unique_ptr<Model> model;
    };
};
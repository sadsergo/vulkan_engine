#pragma once

#include "../vulkan_code/lve_device.hpp"
#include "../model/model.hpp"
#include "../graphics_pipeline/pipeline.hpp"
#include "../vulkan_code/lve_swap_chain.hpp"
#include "window.hpp"

// std
#include <memory>
#include <vector>

namespace lve {
  class App {
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
    void freeCommandBuffers();
    void drawFrame();
    void recreateSwapChain();
    void recordCommandBuffer(int imageIndex);

    LveWindow lveWindow{WIDTH, HEIGHT, "Vulkan Tutorial"};
    LveDevice lveDevice{lveWindow};
    std::unique_ptr<LveSwapChain> lveSwapChain;
    std::unique_ptr<LvePipeline> lvePipeline;
    VkPipelineLayout pipelineLayout;
    std::vector<VkCommandBuffer> commandBuffers;
    std::unique_ptr<LveModel> lveModel;
  };
};
#pragma once

#pragma once

#include "../vulkan_code/lve_device.hpp"
#include "../vulkan_code/lve_swap_chain.hpp"
#include "../glfw_extensions/window.hpp"

// std
#include <memory>
#include <vector>

namespace lve {
  class Renderer {
  public:
    Renderer(LveWindow& window, LveDevice& device) : lveWindow(window), lveDevice(device) {}
    ~Renderer();

    Renderer(const Renderer &) = delete;
    Renderer &operator=(const Renderer &) = delete;

  private:
    void createCommandBuffers();
    void freeCommandBuffers();
    void drawFrame();
    void recreateSwapChain();

    LveWindow&lveWindow;
    LveDevice& lveDevice;
    std::unique_ptr<LveSwapChain> lveSwapChain;
    std::vector<VkCommandBuffer> commandBuffers;
  };
};
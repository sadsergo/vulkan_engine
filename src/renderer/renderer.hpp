#pragma once

#pragma once

#include "../vulkan_code/lve_device.hpp"
#include "../vulkan_code/lve_swap_chain.hpp"
#include "../glfw_extensions/window.hpp"

// std
#include <memory>
#include <vector>
#include <cassert>

namespace lve {
  class Renderer {
  public:
    Renderer(LveWindow& window, LveDevice& device);
    ~Renderer();

    Renderer(const Renderer &) = delete;
    Renderer &operator=(const Renderer &) = delete;

    VkRenderPass getSwapChainRenderPass() const { return lveSwapChain->getRenderPass(); }
    bool isFrameInProgress() const { return isFrameStarted; }
    
    VkCommandBuffer 
    getCurrentCommandBuffer() const 
    {
      assert(isFrameStarted && "Cannot get comand buffer when frame not in progress"); 
      return commandBuffers[currentFrameIndex]; 
    }

    int 
    getFrameIndex() const
    {
      assert(isFrameStarted && "cannot get frame index when frame not in progress");

      return currentFrameIndex;
    }

    VkCommandBuffer beginFrame();
    void endFrame();
    void beginSwapChainRnederPass(VkCommandBuffer commandBuffer);
    void endSwapChainRnederPass(VkCommandBuffer commandBuffer);

  private:
    void createCommandBuffers();
    void freeCommandBuffers();
    void recreateSwapChain();

    LveWindow&window;
    LveDevice& device;
    std::unique_ptr<LveSwapChain> lveSwapChain;
    std::vector<VkCommandBuffer> commandBuffers;

    uint32_t currentImageIndex;
    int currentFrameIndex {0};
    bool isFrameStarted = false;
  };
};
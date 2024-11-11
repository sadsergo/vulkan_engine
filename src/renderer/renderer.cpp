#include "renderer.hpp"

// std
#include <array>
#include <cassert>
#include <stdexcept>

namespace lve {

    Renderer::Renderer(LveWindow& window, LveDevice& device) : window(window), device(device)
    {
        recreateSwapChain();
        createCommandBuffers();
    }

    Renderer::~Renderer() { freeCommandBuffers(); }

    void Renderer::recreateSwapChain() 
    {
        auto extent = window.getExtent();
        
        while (extent.width == 0 || extent.height == 0) 
        {
            extent = window.getExtent();
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(device.device());

        if (lveSwapChain == nullptr) 
        {
            lveSwapChain = std::make_unique<LveSwapChain>(device, extent);
        } 
        else 
        {
            std::shared_ptr<LveSwapChain> oldSwapChain = std::move(lveSwapChain);
            lveSwapChain = std::make_unique<LveSwapChain>(device, extent, oldSwapChain);

            if (!oldSwapChain->compareSwapFormats(*lveSwapChain.get()))
            {
                throw std::runtime_error("swap chain image (or depth) has changed");
            }
        }
    }

    void Renderer::createCommandBuffers() {
        commandBuffers.resize(LveSwapChain::MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = device.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(device.device(), &allocInfo, commandBuffers.data()) !=
            VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }

    void Renderer::freeCommandBuffers() 
    {
        vkFreeCommandBuffers(
        device.device(),
        device.getCommandPool(),
        static_cast<uint32_t>(commandBuffers.size()),
        commandBuffers.data());
        commandBuffers.clear();
    }

    VkCommandBuffer 
    Renderer::beginFrame()
    {
        assert(!isFrameInProgress() && "cannot call beginFrame while in progress");
        
        auto result = lveSwapChain->acquireNextImage(&currentImageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return nullptr;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        isFrameStarted = true;

        auto commandBuffer = getCurrentCommandBuffer();
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        return commandBuffer;
    }

    void 
    Renderer::endFrame()
    {
        assert(isFrameStarted && "cannot call endFrame while frame is not in progress");

        auto commandBuffer = getCurrentCommandBuffer();
        
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }

        auto result = lveSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.wasWindowResized()) 
        {
            window.resetWindowResizedFlag();
            recreateSwapChain();
        } 
        else if (result != VK_SUCCESS) 
        {
            throw std::runtime_error("failed to present swap chain image!");
        }

        isFrameStarted = false;
        currentFrameIndex = (currentFrameIndex + 1) % LveSwapChain::MAX_FRAMES_IN_FLIGHT;
    }

    void 
    Renderer::beginSwapChainRnederPass(VkCommandBuffer commandBuffer)
    {
        assert(isFrameInProgress() && "cannot call beginSwapChainRenderPass if frame is not in progress");
        assert(commandBuffer == getCurrentCommandBuffer() && "cannot begin render pass on comand buffer from different frame");

        VkRenderPassBeginInfo RenderPassInfo{};
        RenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        RenderPassInfo.renderPass = lveSwapChain->getRenderPass();
        RenderPassInfo.framebuffer = lveSwapChain->getFrameBuffer(currentImageIndex);

        RenderPassInfo.renderArea.offset = {0, 0};
        RenderPassInfo.renderArea.extent = lveSwapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};
        RenderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        RenderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &RenderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(lveSwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(lveSwapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{{0, 0}, lveSwapChain->getSwapChainExtent()};
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    }

    void Renderer::endSwapChainRnederPass(VkCommandBuffer commandBuffer)
    {
        assert(isFrameInProgress() && "cannot call endSwapChainRenderPass if frame is not in progress");
        assert(commandBuffer == getCurrentCommandBuffer() && "cannot end render pass on comand buffer from different frame");

        vkCmdEndRenderPass(commandBuffer);
    }

};
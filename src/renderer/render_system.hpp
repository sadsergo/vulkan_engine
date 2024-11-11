#pragma once

#include "../vulkan_code/lve_device.hpp"
#include "../model/model.hpp"
#include "../graphics_pipeline/pipeline.hpp"
#include "../game_object/game_object.hpp"
#include "../glfw_extensions/camera.hpp"

// std
#include <memory>
#include <vector>

namespace lve {
  class RenderSystem {
  public:
    RenderSystem(LveDevice &device, VkRenderPass renderPass);
    ~RenderSystem();

    RenderSystem(const RenderSystem &) = delete;
    RenderSystem &operator=(const RenderSystem &) = delete;
    void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject> &gameObjects, const Camera &camera);

  private:
    void createPipelineLayout();
    void createPipeline(VkRenderPass renderPass);

    LveDevice &device;
    std::unique_ptr<LvePipeline> lvePipeline;
    VkPipelineLayout pipelineLayout;
  };
};
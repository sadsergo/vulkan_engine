#include "app.hpp"

// std
#include <array>
#include <cassert>
#include <stdexcept>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace lve {

  struct SimplePushConstantData
  {
    glm::mat2 transform {1.f};
    glm::vec2 offset;
    alignas(16) glm::vec3 color;
  };

  App::App() {
    loadGameObjects();
    createPipelineLayout();
    createPipeline();
  }

  App::~App() { vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr); }

  void App::run() {
    while (!window.shouldClose()) {
      glfwPollEvents();
      
      if (auto commandBuffer = renderer.beginFrame())
      {
        /*
          begin offscreen shadow pass
          render shadow casting objects
          end offscreen shadow pass
        */
       
        renderer.beginSwapChainRnederPass(commandBuffer);
        renderGameObjects(commandBuffer);
        renderer.endSwapChainRnederPass(commandBuffer);
        renderer.endFrame();
      }
    }

    vkDeviceWaitIdle(device.device());
  }

  void App::loadGameObjects() {
    std::vector<Model::Vertex> vertices {
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};

    auto model = std::make_shared<Model>(device, vertices);
    
    auto triangle = GameObject::createGameObject();
    triangle.model = model;
    triangle.color = {.1f, .8f, .1f};
    triangle.transform2D.translation.x = .2f;
    triangle.transform2D.scale = {2.f, .5f};
    triangle.transform2D.rotation = .25f * glm::two_pi<float>();

    gameObjects.push_back(std::move(triangle));
  }

  void App::createPipelineLayout() {
    VkPushConstantRange pushConstantRange {};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(SimplePushConstantData);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
    if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
        VK_SUCCESS) {
      throw std::runtime_error("failed to create pipeline layout!");
    }
  }

  void App::createPipeline() {
    assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig{};
    LvePipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = renderer.getSwapChainRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout;
    lvePipeline = std::make_unique<LvePipeline>(
        device,
        "shaders/simple_shader.vert.spv",
        "shaders/simple_shader.frag.spv",
        pipelineConfig);
  }

  void
  App::renderGameObjects(VkCommandBuffer commandBuffer)
  {
    lvePipeline->bind(commandBuffer);

    for (auto &obj: gameObjects)
    {
      obj.transform2D.rotation = glm::mod(obj.transform2D.rotation + 0.01, glm::two_pi<double>());
      SimplePushConstantData push {};
      push.offset = obj.transform2D.translation;
      push.color = obj.color;
      push.transform = obj.transform2D.mat2();

      vkCmdPushConstants(
        commandBuffer, 
        pipelineLayout, 
        VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 
        0, 
        sizeof(SimplePushConstantData), 
        &push);

      obj.model->bind(commandBuffer);
      obj.model->draw(commandBuffer);
    }
  }

};
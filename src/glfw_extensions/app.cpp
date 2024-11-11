#include "app.hpp"

// std
#include <array>
#include <cassert>
#include <stdexcept>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "../renderer/render_system.hpp"

namespace lve {

  App::App() 
  {
    loadGameObjects();
  }

  App::~App() {}

  void App::run() 
  {
    RenderSystem renderSystem {device, renderer.getSwapChainRenderPass()};

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
        renderSystem.renderGameObjects(commandBuffer, gameObjects);
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

};
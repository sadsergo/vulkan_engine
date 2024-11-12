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
    
    Camera camera {};
    // camera.setViewDirection(glm::vec3{0}, glm::vec3{0.5, 0, 1});
    camera.setViewTarget(glm::vec3{-1, -2, 2}, glm::vec3{0, 0, 2.5});

    while (!window.shouldClose()) {
      glfwPollEvents();
      
      float aspect = renderer.getAspectRatio();
      // camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
      camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

      if (auto commandBuffer = renderer.beginFrame())
      {
        /*
          begin offscreen shadow pass
          render shadow casting objects
          end offscreen shadow pass
        */
       
        renderer.beginSwapChainRnederPass(commandBuffer);
        renderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
        renderer.endSwapChainRnederPass(commandBuffer);
        renderer.endFrame();
      }
    }

    vkDeviceWaitIdle(device.device());
  }

  std::unique_ptr<Model> createCubeModel(LveDevice& device, glm::vec3 offset) {
    std::vector<Model::Vertex> vertices {
    // left face (white)
    {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
    {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
    {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
    {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
    {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
    {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
    // right face (yellow)
    {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
    {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
    {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
    {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
    {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
    {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
    // top face (orange, remember y axis points down)
    {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
    {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
    {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
    {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
    {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
    {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
    // bottom face (red)
    {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
    {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
    {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
    {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
    {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
    {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
    // nose face (blue)
    {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
    {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
    {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
    {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
    {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
    {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
    // tail face (green)
    {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
    {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
    {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
    {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
    {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
    {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
    };

    for (auto& v : vertices) {
      v.position += offset;
    }
    
    return std::make_unique<Model>(device, vertices);
  }

  void App::loadGameObjects() {
      std::shared_ptr<Model> lveModel = createCubeModel(device, {.0f, .0f, .0f});
      auto cube = GameObject::createGameObject();
      cube.model = lveModel;
      cube.transform.translation = {.0f, .0f, 2.5f};
      cube.transform.scale = {.5f, .5f, .5f};
      gameObjects.push_back(std::move(cube));
  }

};
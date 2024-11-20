#include "app.hpp"

// std
#include <array>
#include <cassert>
#include <stdexcept>
#include <chrono>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "../renderer/render_system.hpp"
#include "keyboard.hpp"

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
    auto viewerObject = GameObject::createGameObject();
    KeyboardMovementController cameraController {};

    auto currentTime = std::chrono::high_resolution_clock::now();

    while (!window.shouldClose()) {
      glfwPollEvents();
      
      auto newTime = std::chrono::high_resolution_clock::now();
      float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
      currentTime = newTime;

      cameraController.moveInPlaneXZ(window.getGLFWwindow(), frameTime, viewerObject);
      camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

      float aspect = renderer.getAspectRatio();
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

  void App::loadGameObjects() {
      std::shared_ptr<Model> lveModel = Model::createModelFromFile(device, "/home/sergei/Desktop/vulkan/vulkan_engine/models/smooth_vase.obj");
      auto gameObj = GameObject::createGameObject();
      gameObj.model = lveModel;
      gameObj.transform.translation = {.0f, .0f, 2.5f};
      gameObj.transform.scale = glm::vec3(3);
      gameObjects.push_back(std::move(gameObj));
  }

};
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
#include "../uniform_buffer/buffer.hpp"
#include "keyboard.hpp"

namespace lve {

  struct GlobalUbo
  {
    glm::mat4 projectionView {1.f};
    glm::vec3 lightDirection = glm::normalize(glm::vec3{1.f, -3.f, -1.f});
  };

  App::App() 
  {
    loadGameObjects();
  }

  App::~App() {}

  void App::run() 
  {
    Buffer globalUboBuffer {
      device,
      sizeof(GlobalUbo),
      LveSwapChain::MAX_FRAMES_IN_FLIGHT,
      VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
      device.properties.limits.minUniformBufferOffsetAlignment
    };
    globalUboBuffer.map();



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
      camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 1000.f);

      if (auto commandBuffer = renderer.beginFrame())
      {
        int frameIndex = renderer.getFrameIndex();
        
        //  update
        GlobalUbo ubo {};
        ubo.projectionView = camera.getProjection() * camera.getView();
        globalUboBuffer.writeToIndex(&ubo, frameIndex);
        globalUboBuffer.flushIndex(frameIndex);

        //  render
        renderer.beginSwapChainRnederPass(commandBuffer);
        renderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
        renderer.endSwapChainRnederPass(commandBuffer);
        renderer.endFrame();
      }
    }

    vkDeviceWaitIdle(device.device());
  }

  void App::loadGameObjects() {
      std::shared_ptr<Model> lveModel = Model::createModelFromFile(device, "models/rangerover.obj");
      auto gameObj = GameObject::createGameObject();
      gameObj.model = lveModel;
      gameObj.transform.translation = {.0f, .5f, 2.5f};
      gameObj.transform.scale = glm::vec3(0.01);
      gameObjects.push_back(std::move(gameObj));
  }
};
#pragma once

#include "../vulkan_code/lve_device.hpp"
#include "../model/model.hpp"
#include "../game_object/game_object.hpp"
#include "../renderer/renderer.hpp"
#include "camera.hpp"
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
    void loadGameObjects();

    LveWindow window {WIDTH, HEIGHT, "Game Engine"};
    LveDevice device {window};
    Renderer renderer {window, device};
    std::vector<GameObject> gameObjects;
  };
};
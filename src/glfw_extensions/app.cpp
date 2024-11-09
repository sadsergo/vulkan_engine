#include "app.hpp"

namespace lve 
{
    void App::run()
    {
        while (!window.shouldClose())
        {
            glfwPollEvents();
        }
    }
};
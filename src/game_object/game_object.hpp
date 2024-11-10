#pragma once

#include "../model/model.hpp"

#include <memory>

namespace lve 
{
    struct Transform2DComponent
    {
        glm::vec2 translation;  //  (position offset)
        glm::vec2 scale {1.f, 1.f};
        float rotation;

        glm::mat2 
        mat2() 
        { 
            const float s = glm::sin(rotation);
            const float c = glm::cos(rotation);

            glm::mat2 rotMatrix {{c, s}, {-s, c}};
            glm::mat2 scaleMat {{scale.x, 0}, {0, scale.y}};
            return rotMatrix * scaleMat; 
        }
    };

    class GameObject
    {
    public:
        using id_t = unsigned int;
        
        std::shared_ptr<Model> model {};
        glm::vec3 color {};
        Transform2DComponent transform2D {};

        static 
        GameObject createGameObject()
        {
            id_t currentId = 0;
            return GameObject(currentId++);
        }

        GameObject(const GameObject &) = delete;
        GameObject &operator=(const GameObject &) = delete;
        GameObject(GameObject &&) = default;
        GameObject &operator=(GameObject &&) = default;

        id_t getId() const { return id; }
    private:
        GameObject(id_t objId) : id(objId) {}

        id_t id;
    };
};
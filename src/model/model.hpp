#pragma once

#include "../vulkan_code/lve_device.hpp"
#include "../uniform_buffer/buffer.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std
#include <memory>
#include <vector>

namespace lve {
  class Model {
  public:
    struct Vertex {
      glm::vec3 position {};
      glm::vec3 color {};
      glm::vec3 normal {};
      glm::vec2 uv {};

      static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
      static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
    };

    struct Builder
    {
      std::vector<Vertex> vertices {};
      std::vector<uint32_t> indices {};

      void loadModel(const std::string filepath);
    };

    Model(LveDevice &device, const Model::Builder& builder);
    ~Model();

    static std::unique_ptr<Model> createModelFromFile(LveDevice& device, const std::string& filepath);

    Model(const Model &) = delete;
    Model &operator=(const Model &) = delete;

    void bind(VkCommandBuffer commandBuffer);
    void draw(VkCommandBuffer commandBuffer);

  private:
    void createVertexBuffer(const std::vector<Vertex> &vertices);
    void createIndexBuffer(const std::vector<uint32_t> &indices);

    LveDevice &device;

    std::unique_ptr<Buffer> vertexBuffer;
    uint32_t vertexCount;

    bool hasIndexBuffer = false;
    std::unique_ptr<Buffer> indexBuffer;
    uint32_t indexCount;
  };
};
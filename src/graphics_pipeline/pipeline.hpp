#pragma once 

#include <string>
#include <vector>
#include <cstdint>

#include "../vulkan_code/lve_device.hpp"
#include "../model/model.hpp"

namespace lve
{
    struct PipelineConfigInfo 
    {
        VkViewport viewport;
        VkRect2D scissor;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };

    class Pipeline
    {
    public:
        Pipeline(
            LveDevice &device, 
            const std::string &vertFilePath, 
            const std::string &fragFilePath, 
            const PipelineConfigInfo &configInfo);
        ~Pipeline();

        Pipeline(const Pipeline &) = delete;
        void operator=(const Pipeline &) = delete;

        void bind(VkCommandBuffer comandBuffer);
        static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

    private:
        static std::vector<char> readFile(const std::string &filePath);
        void createGraphicsPipeline(const std::string &vertFilePath, const std::string &fragFilePath, const PipelineConfigInfo& configInfo);
        void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

        LveDevice& device;
        VkPipeline graphicsPipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
    };
};
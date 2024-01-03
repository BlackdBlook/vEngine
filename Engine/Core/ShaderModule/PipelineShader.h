#pragma once
#include <string>
#include <vulkan/vulkan_core.h>


class RenderPipelineInfo
{
public:
    std::string VertShaderName;
    std::string FragShaderName;
    virtual VkPipelineVertexInputStateCreateInfo* PipelineVertexInputStateCreateInfo();
    virtual VkPipelineInputAssemblyStateCreateInfo* PipelineInputAssemblyStateCreateInfo();
    virtual VkPipelineViewportStateCreateInfo* PipelineViewportStateCreateInfo();
    virtual VkPipelineRasterizationStateCreateInfo* PipelineRasterizationStateCreateInfo();
    virtual VkPipelineMultisampleStateCreateInfo* PipelineMultisampleStateCreateInfo();
    virtual VkPipelineColorBlendAttachmentState* PipelineColorBlendAttachmentState();
    virtual VkPipelineColorBlendStateCreateInfo* PipelineColorBlendStateCreateInfo();
    virtual VkPipelineDynamicStateCreateInfo* PipelineDynamicStateCreateInfo();
    virtual VkPipelineLayout PipelineLayout();
    virtual VkRenderPass PipelineRenderPass();
    virtual ~RenderPipelineInfo(){}

    std::string GetName();
};


class RenderPipeline
{
    VkPipelineBindPoint PipelineBindPoint;
    VkPipeline graphicsPipeline = nullptr;
public:
    RenderPipeline();
    RenderPipeline(const char* ShaderName);
    RenderPipeline(const char* VertShaderName, const char* FragShaderName);
    RenderPipeline(RenderPipelineInfo* info);

    void Init(RenderPipelineInfo* Info);

    void CmdBind(VkCommandBuffer commandBuffer);
    
    ~RenderPipeline();
};

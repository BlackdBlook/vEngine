#pragma once
#include <vulkan/vulkan_core.h>

class RenderPipeline
{
    VkPipelineBindPoint PipelineBindPoint;
    VkPipeline graphicsPipeline;
    void init(const char* c1, const char* c2);
public:
    RenderPipeline(const char* ShaderName);
    RenderPipeline(const char* VertShaderName, const char* FragShaderName);

    void Bind(VkCommandBuffer commandBuffer);
    
    ~RenderPipeline();
};

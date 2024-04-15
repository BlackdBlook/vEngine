#pragma once
#include <vector>

#include "Engine/Core/ShaderModule/PipelineShader.h"
#include "Engine/Core/ShaderModule/Shader.h"
#include "Engine/Core/UniformBuffer/UniformBuffer.h"


class vShader;

class MaterialRenderPipelineInfo
{
    mutable VkDescriptorSetLayout descriptorSetLayout = nullptr;
    mutable VkPipelineLayout pipelineLayout = nullptr;
public:

    SPtr<vShader> VertShader;
    SPtr<vShader> FragShader;
    MaterialRenderPipelineInfo();
    
    MaterialRenderPipelineInfo(const string& shaderName, ShaderCodeType codeType = ShaderCodeType::GLSL);
    
    MaterialRenderPipelineInfo(const string& VertShaderName, const string& FragShaderName, ShaderCodeType codeType = ShaderCodeType::GLSL);

    virtual ~MaterialRenderPipelineInfo();
    
    virtual VkPipelineVertexInputStateCreateInfo* PipelineVertexInputStateCreateInfo() const;
    virtual VkPipelineInputAssemblyStateCreateInfo* PipelineInputAssemblyStateCreateInfo()const;
    virtual VkPipelineViewportStateCreateInfo* PipelineViewportStateCreateInfo()const;
    virtual VkPipelineRasterizationStateCreateInfo* PipelineRasterizationStateCreateInfo()const;
    virtual VkPipelineMultisampleStateCreateInfo* PipelineMultisampleStateCreateInfo()const;
    virtual VkPipelineColorBlendAttachmentState* PipelineColorBlendAttachmentState()const;
    virtual VkPipelineColorBlendStateCreateInfo* PipelineColorBlendStateCreateInfo()const;
    virtual VkPipelineDynamicStateCreateInfo* PipelineDynamicStateCreateInfo()const;
    virtual VkPipelineLayout PipelineLayout()const;
    virtual VkRenderPass PipelineRenderPass()const;
    virtual uint32 PipelineSubpass();
    virtual VkPipeline PipelineBasePipelineHandle()const;

    virtual VkDescriptorSetLayout MakeVkDescriptorSetLayout() const;
    virtual std::vector<UniformBuffer> MakeUniformBuffers() const;
};

class MaterialRenderPipeline
{
    VkPipelineBindPoint PipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    VkPipeline graphicsPipeline = nullptr;
public:
    VkGraphicsPipelineCreateInfo pipelineInfo{};
    
    MaterialRenderPipeline();

    void Init(MaterialRenderPipelineInfo* Info);

    void CmdBind(VkCommandBuffer commandBuffer);
    
    ~MaterialRenderPipeline();
};


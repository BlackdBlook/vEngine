#pragma once
#include <vector>

#include "Engine/Core/ShaderModule/Shader.h"
#include "Engine/Core/UniformBuffer/UniformBuffer.h"


class Texture2D;
class vShader;

class MaterialRenderPipelineInfo
{
protected:
    mutable VkDescriptorSetLayout descriptorSetLayout = nullptr;
    mutable VkPipelineLayout pipelineLayout = nullptr;
public:

    SPtr<vShader> VertShader;
    SPtr<vShader> FragShader;
    MaterialRenderPipelineInfo();

    MaterialRenderPipelineInfo(const MaterialRenderPipelineInfo& other) = delete;
    
    MaterialRenderPipelineInfo(MaterialRenderPipelineInfo&& other) = default;
    
    MaterialRenderPipelineInfo(const string& shaderName, ShaderCodeType codeType = ShaderCodeType::HLSL);
    
    MaterialRenderPipelineInfo(const string& VertShaderName, const string& FragShaderName, ShaderCodeType codeType = ShaderCodeType::HLSL);

    virtual ~MaterialRenderPipelineInfo();
    
    virtual VkPipelineVertexInputStateCreateInfo* PipelineVertexInputStateCreateInfo() const;
    virtual VkPipelineInputAssemblyStateCreateInfo* PipelineInputAssemblyStateCreateInfo()const;
    virtual VkPipelineViewportStateCreateInfo* PipelineViewportStateCreateInfo()const;
    virtual VkPipelineRasterizationStateCreateInfo* PipelineRasterizationStateCreateInfo()const;
    virtual VkPipelineMultisampleStateCreateInfo* PipelineMultisampleStateCreateInfo()const;
    virtual VkPipelineColorBlendAttachmentState* PipelineColorBlendAttachmentState()const;
    virtual VkPipelineColorBlendStateCreateInfo* PipelineColorBlendStateCreateInfo()const;
    virtual VkPipelineDepthStencilStateCreateInfo* PipelineDepthStencilStat()const;
    virtual VkPipelineDynamicStateCreateInfo* PipelineDynamicStateCreateInfo()const;
    virtual VkPipelineLayout PipelineLayout()const;
    virtual VkRenderPass PipelineRenderPass()const;
    virtual uint32 PipelineSubpass();
    virtual VkPipeline PipelineBasePipelineHandle()const;

    virtual VkDescriptorSetLayout MakeVkDescriptorSetLayout() const;
    virtual std::vector<SPtr<UniformBuffer>> MakeUniformBuffers() const;
    virtual void MakeInputTextures(std::unordered_map<string, SPtr<Texture2D>>& out) const;
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


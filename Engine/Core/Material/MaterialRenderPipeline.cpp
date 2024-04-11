#include "MaterialRenderPipeline.h"

#include <array>

#include "Engine/vEngine.h"
#include "Engine/Core/MemoryBuffer/MeshVertexBuffer.h"
#include "Engine/Core/ShaderModule/Shader.h"
#include "LogPrinter/Log.h"

MaterialRenderPipelineInfo::MaterialRenderPipelineInfo()
{
    
}

MaterialRenderPipelineInfo::MaterialRenderPipelineInfo(const string& shaderName)
{
    VertShader = NewSPtr<vShader>(shaderName.c_str(), ShaderType::Vertex);
    FragShader = NewSPtr<vShader>(shaderName.c_str(), ShaderType::Fragment);
}

MaterialRenderPipelineInfo::MaterialRenderPipelineInfo(const string& VertShaderName, const string& FragShaderName)
{
    VertShader = NewSPtr<vShader>(VertShaderName.c_str(), ShaderType::Vertex);
    FragShader = NewSPtr<vShader>(FragShaderName.c_str(), ShaderType::Fragment);
}

MaterialRenderPipelineInfo::~MaterialRenderPipelineInfo()
{
    if(descriptorSetLayout)
    {
        vkDestroyDescriptorSetLayout(GDevice, descriptorSetLayout, nullptr);
    }

    if(pipelineLayout)
    {
        vkDestroyPipelineLayout(GDevice, pipelineLayout, nullptr);
    }
}

VkPipelineVertexInputStateCreateInfo* MaterialRenderPipelineInfo::PipelineVertexInputStateCreateInfo()const
{
    static VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    static auto bindingDescription = MeshVertex::getBindingDescription();
    static auto attributeDescriptions =
        MeshVertex::getAttributeDescriptions();
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    return &vertexInputInfo;
}

VkPipelineInputAssemblyStateCreateInfo* MaterialRenderPipelineInfo::PipelineInputAssemblyStateCreateInfo()const
{
    static VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;
    return &inputAssembly;
}

VkPipelineViewportStateCreateInfo* MaterialRenderPipelineInfo::PipelineViewportStateCreateInfo()const
{
    static VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;
    return &viewportState;
}

VkPipelineRasterizationStateCreateInfo* MaterialRenderPipelineInfo::PipelineRasterizationStateCreateInfo()const
{
    static VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    // 背面剔除
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    // 逆时针为正面
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    return &rasterizer;
}

VkPipelineMultisampleStateCreateInfo* MaterialRenderPipelineInfo::PipelineMultisampleStateCreateInfo()const
{
    static VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    return &multisampling;
}

VkPipelineColorBlendAttachmentState* MaterialRenderPipelineInfo::PipelineColorBlendAttachmentState()const
{
    static VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    return &colorBlendAttachment;
}

VkPipelineColorBlendStateCreateInfo* MaterialRenderPipelineInfo::PipelineColorBlendStateCreateInfo()const
{
    static VkPipelineColorBlendAttachmentState* colorBlendAttachment =
        PipelineColorBlendAttachmentState();
    
    static VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;
    return &colorBlending;
}

VkPipelineDynamicStateCreateInfo* MaterialRenderPipelineInfo::PipelineDynamicStateCreateInfo()const
{
    static std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };
    static VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    return &dynamicState;
}

VkPipelineLayout MaterialRenderPipelineInfo::PipelineLayout()const
{
    if(pipelineLayout)
    {
        return pipelineLayout;
    }
    //创建pipeline layout，指定shader uniform值
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;


    if (vkCreatePipelineLayout(GDevice, &pipelineLayoutInfo,
        nullptr, &pipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create pipeline layout!");
    }
    
    return pipelineLayout;
}

VkRenderPass MaterialRenderPipelineInfo::PipelineRenderPass()const
{
    return GlobalVkRenderPass;
}

uint32 MaterialRenderPipelineInfo::PipelineSubpass()
{
    return 0;
}

VkPipeline MaterialRenderPipelineInfo::PipelineBasePipelineHandle()const
{
    return VK_NULL_HANDLE;
}

VkDescriptorSetLayout MaterialRenderPipelineInfo::MakeVkDescriptorSetLayout() const
{
    if(descriptorSetLayout)
    {
        return descriptorSetLayout;
    }

    std::vector<VkDescriptorSetLayoutBinding> out;
    VertShader->FillDescriptorSetLayoutBinding(out);
    FragShader->FillDescriptorSetLayoutBinding(out);
 
    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = (uint32_t)out.size();
    layoutInfo.pBindings = out.data();
 
    if (vkCreateDescriptorSetLayout(GDevice, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor set layout!");
    }


    return descriptorSetLayout;
}

std::vector<UniformBuffer> MaterialRenderPipelineInfo::MakeUniformBuffers() const
{
    std::vector<UniformBuffer> out;

    for(auto& blocks :
        VertShader->ShaderUniformBufferBlocks.UniformBlocks)
    {
        UniformBuffer& buffer = out.emplace_back();
        buffer.Init(blocks.second.Size, blocks.first, blocks.second.Bind);
    }

    for(auto& blocks :
        FragShader->ShaderUniformBufferBlocks.UniformBlocks)
    {
        UniformBuffer& buffer = out.emplace_back();
        buffer.BlockName = blocks.first;
        buffer.Init(blocks.second.Size, blocks.first, blocks.second.Bind);
    }
    
    return out;
}

MaterialRenderPipeline::MaterialRenderPipeline()
{
    
}

void MaterialRenderPipeline::CmdBind(VkCommandBuffer commandBuffer)
{
    vkCmdBindPipeline(commandBuffer, PipelineBindPoint, graphicsPipeline);
}

MaterialRenderPipeline::~MaterialRenderPipeline()
{
    vkDestroyPipeline(GDevice, graphicsPipeline, nullptr);
}

void MaterialRenderPipeline::Init(MaterialRenderPipelineInfo* Info)
{
    if(!Info->VertShader || !Info->FragShader)
    {
        LOG("Shader Name Is Empty !!!");
        return;
    }
    
    VkPipelineShaderStageCreateInfo shaderStages[] =
        {Info->VertShader->GetStageInfo(), Info->FragShader->GetStageInfo()};
    
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = Info->PipelineVertexInputStateCreateInfo();
    pipelineInfo.pInputAssemblyState = Info->PipelineInputAssemblyStateCreateInfo();
    pipelineInfo.pViewportState = Info->PipelineViewportStateCreateInfo();
    pipelineInfo.pRasterizationState = Info->PipelineRasterizationStateCreateInfo();
    pipelineInfo.pMultisampleState = Info->PipelineMultisampleStateCreateInfo();
    pipelineInfo.pColorBlendState = Info->PipelineColorBlendStateCreateInfo();
    pipelineInfo.pDynamicState = Info->PipelineDynamicStateCreateInfo();
    pipelineInfo.layout = Info->PipelineLayout();
    pipelineInfo.renderPass = Info->PipelineRenderPass();
    pipelineInfo.subpass = Info->PipelineSubpass();
    pipelineInfo.basePipelineHandle = Info->PipelineBasePipelineHandle();

    if (vkCreateGraphicsPipelines(GDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
    }
}

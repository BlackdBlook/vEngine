#include "PipelineShader.h"

#include <vector>

#include "Shader.h"
#include "Engine/vEngine.h"
#include "Engine/SubSystem/AssetSystem/AssetSystem.h"
#include "Engine/Toolkit/FileToolKit/FileToolKit.h"
#include "LogPrinter/Log.h"

VkPipelineVertexInputStateCreateInfo* RenderPipelineInfo::PipelineVertexInputStateCreateInfo()
{
    static VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    return &vertexInputInfo;
}

VkPipelineInputAssemblyStateCreateInfo* RenderPipelineInfo::PipelineInputAssemblyStateCreateInfo()
{
    static VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;
    return &inputAssembly;
}

VkPipelineViewportStateCreateInfo* RenderPipelineInfo::PipelineViewportStateCreateInfo()
{
    static VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;
    return &viewportState;
}

VkPipelineRasterizationStateCreateInfo* RenderPipelineInfo::PipelineRasterizationStateCreateInfo()
{
    static VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    // 背面剔除
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    // Vulkan默认情况下逆时针为正面
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    return &rasterizer;
}

VkPipelineMultisampleStateCreateInfo* RenderPipelineInfo::PipelineMultisampleStateCreateInfo()
{
    static VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    return &multisampling;
}

VkPipelineColorBlendAttachmentState* RenderPipelineInfo::PipelineColorBlendAttachmentState()
{
    static VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    return &colorBlendAttachment;
}

VkPipelineColorBlendStateCreateInfo* RenderPipelineInfo::PipelineColorBlendStateCreateInfo()
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

VkPipelineDynamicStateCreateInfo* RenderPipelineInfo::PipelineDynamicStateCreateInfo()
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

VkPipelineLayout RenderPipelineInfo::PipelineLayout()
{
    return GlobalVkPipeLineLayout;
}

VkRenderPass RenderPipelineInfo::PipelineRenderPass()
{
    return GlobalVkRenderPass;
}

uint32 RenderPipelineInfo::PipelineSubpass()
{
    return 0;
}

VkPipeline RenderPipelineInfo::PipelineBasePipelineHandle()
{
    return VK_NULL_HANDLE;
}

std::string RenderPipelineInfo::GetName()
{
    return VertShaderName + "_" + FragShaderName;
}

void RenderPipeline::Init(RenderPipelineInfo* Info)
{
    if(graphicsPipeline != nullptr)
    {
        // 限定仅初始化一次
        LOG("graphicsPipeline inited!!!", Info->GetName());
        assert(graphicsPipeline != nullptr);
        return;
    }

    if(Info->VertShaderName.empty() || Info->FragShaderName.empty())
    {
        LOG("Shader Name Is Empty !!!");
        return;
    }

    
    vShader vertShaderModule =
        vShader(Info->VertShaderName.c_str(), ShaderType::Vertex);

    vShader fragShaderModule =
        vShader(Info->FragShaderName.c_str(), ShaderType::Fragment);

    VkPipelineShaderStageCreateInfo shaderStages[] =
        {vertShaderModule.GetStageInfo(), fragShaderModule.GetStageInfo()};

    
    VkGraphicsPipelineCreateInfo pipelineInfo{};
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
    layout = Info->PipelineLayout();
    pipelineInfo.layout = layout;
    pipelineInfo.renderPass = Info->PipelineRenderPass();
    pipelineInfo.subpass = Info->PipelineSubpass();
    pipelineInfo.basePipelineHandle = Info->PipelineBasePipelineHandle();
    
    if (vkCreateGraphicsPipelines(GDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
    }

}

RenderPipeline::RenderPipeline()
{
}

RenderPipeline::RenderPipeline(const char* ShaderName)
{
    RenderPipelineInfo info;
    info.FragShaderName = ShaderName;
    info.VertShaderName = ShaderName;
    Init(&info);
}

RenderPipeline::RenderPipeline(const char* VertShaderName, const char* FragShaderName)
{
    RenderPipelineInfo info;
    info.FragShaderName = FragShaderName;
    info.VertShaderName = VertShaderName;
    Init(&info);
}

RenderPipeline::RenderPipeline(RenderPipelineInfo* info)
{
    Init(info);
}

void RenderPipeline::CmdBind(VkCommandBuffer commandBuffer)
{
    vkCmdBindPipeline(commandBuffer, PipelineBindPoint, graphicsPipeline);
}

RenderPipeline::~RenderPipeline()
{
    vkDestroyPipeline(GDevice, graphicsPipeline, nullptr);
    if(layout != GlobalVkPipeLineLayout)
    {
        vkDestroyPipelineLayout(GDevice, layout, nullptr);
    }
}

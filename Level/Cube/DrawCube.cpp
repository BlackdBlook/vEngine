#include "DrawCube.h"

#include <array>
#include <stdexcept>

#include "Engine/vEngine.h"
#include "Engine/Core/Component/Component.h"
#include "Engine/Core/FrameInfo/RenderInfo.h"
#include "Engine/Core/MemoryBuffer/MemoryBuffer.h"
#include "Engine/Core/Object/Object.h"
#include "Engine/Core/ShaderModule/PipelineShader.h"
#include "Meshs/Box/BoxVertices.h"

struct Vertex
{
    glm::vec3 Pos;
    glm::vec3 Normal;
    glm::vec2 TexCoords;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription {};

        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        
        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, Pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, Normal);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, TexCoords);
        return attributeDescriptions;
    }
};

namespace
{
    class RenderPipelineInfo2 : public RenderPipelineInfo
    {
        VkPipelineVertexInputStateCreateInfo* PipelineVertexInputStateCreateInfo() override;
    };
}

class Cube : public Component
{
    RenderPipeline pipeline;
    
    MemoryBuffer buffer;
public:
    Cube();
    
    void Draw(const RenderInfo& RenderInfo) override;
};

VkPipelineVertexInputStateCreateInfo* RenderPipelineInfo2::PipelineVertexInputStateCreateInfo()
{
    static VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    static auto bindingDescription = Vertex::getBindingDescription();
    static auto attributeDescriptions =
        Vertex::getAttributeDescriptions();
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    return &vertexInputInfo;
}

VkDescriptorSetLayout createDescriptorSetLayout() {

    VkDescriptorSetLayout ans;
    
    VkDescriptorSetLayoutBinding uboLayoutBinding = {};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    uboLayoutBinding.pImmutableSamplers = nullptr;
 
    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &uboLayoutBinding;
 
    if (vkCreateDescriptorSetLayout(GlobalVkLogicDevice, &layoutInfo, nullptr, &ans) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }

    return ans;
}


Cube::Cube() : buffer(sizeof(BoxVertices[0]), BoxVertices)
{
    RenderPipelineInfo2 info;
    info.VertShaderName = "DrawCube";
    info.FragShaderName = "DrawCube";
    pipeline.Init(&info);
    
}

void Cube::Draw(const RenderInfo& RenderInfo)
{
    Component::Draw(RenderInfo);

        Component::Draw(RenderInfo);

    pipeline.CmdBind(RenderInfo.CommmandBuffer);

    buffer.CmdBind(RenderInfo.CommmandBuffer);

    vkCmdDraw(RenderInfo.CommmandBuffer, sizeof(BoxVertices) / 8, 1, 0, 0);
    
}

void DrawCube::Init()
{
    Level::Init();

    {
        auto obj = NewObject();
        obj->Attach(NewSPtr<Cube>());
    }
}

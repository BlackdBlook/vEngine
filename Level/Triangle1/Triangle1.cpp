#include "Triangle1.h"

#include <array>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Engine/Core/Component/Component.h"
#include "Engine/Core/FrameInfo/RenderInfo.h"
#include "Engine/Core/MemoryBuffer/MemoryBuffer.h"
#include "Engine/Core/Object/Object.h"
#include "Engine/Core/ShaderModule/PipelineShader.h"

struct Vertex
{
    glm::vec2 pos;
    glm::vec3 color;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription {};

        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        
        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);
        return attributeDescriptions;
    }
};

const std::vector<Vertex> vertices = {
    {{0.0f, -0.5f}, {1.0f, 1.0f, 1.0f}},
    {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
};

class Triangle1Comp : public Component
{
public:
    Triangle1Comp();
    
    void Draw(const RenderInfo& RenderInfo) override;

    RenderPipeline pipeline;

    MemoryBuffer buffer;
};

namespace
{
    class RenderPipelineInfo2 : public RenderPipelineInfo
    {
        VkPipelineVertexInputStateCreateInfo* PipelineVertexInputStateCreateInfo() override;
    };
}

Triangle1Comp::Triangle1Comp() : buffer(sizeof(vertices[0]) * vertices.size(), vertices.data())
{
    RenderPipelineInfo2 info;
    info.VertShaderName = "Triangle1";
    info.FragShaderName = "Triangle1";
    pipeline.Init(&info);
}

void Triangle1Comp::Draw(const RenderInfo& RenderInfo)
{
    Component::Draw(RenderInfo);

    pipeline.CmdBind(RenderInfo.CommmandBuffer);

    buffer.CmdBind(RenderInfo.CommmandBuffer);
    
    vkCmdDraw(RenderInfo.CommmandBuffer, 3, 1, 0, 0);
}

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

void Triangle1::Init()
{
    Level::Init();

    {
        auto obj = NewObject();
        obj->AttachComponent(NewSPtr<Triangle1Comp>());
    }
    
}

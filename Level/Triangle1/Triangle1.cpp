#include "Triangle1.h"

#include <array>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Engine/Core/Component/Component.h"
#include "Engine/Core/FrameInfo/RenderInfo.h"
#include "Engine/Core/MemoryBuffer/MeshVertexBuffer.h"
#include "Engine/Core/Object/Object.h"
#include "Engine/Core/ShaderModule/PipelineShader.h"
#include "LogPrinter/Log.h"

struct TriangleVertex
{
    glm::vec2 pos;
    glm::vec3 color;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription {};

        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(TriangleVertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        
        volatile auto x = bindingDescription.stride;
        volatile auto x2 = bindingDescription.stride;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(TriangleVertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(TriangleVertex, color);

        return attributeDescriptions;
    }
};

const std::vector<TriangleVertex> vertices = {
    {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{-0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
};

namespace
{
    class RenderPipelineInfo2 : public RenderPipelineInfo
    {
        VkPipelineVertexInputStateCreateInfo* PipelineVertexInputStateCreateInfo() override;
    };
}
VkPipelineVertexInputStateCreateInfo* RenderPipelineInfo2::PipelineVertexInputStateCreateInfo()
{
    static VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    
    static auto bindingDescription = TriangleVertex::getBindingDescription();
    static auto attributeDescriptions =
        TriangleVertex::getAttributeDescriptions();
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = 2;
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
    
    return &vertexInputInfo;
}

class Triangle1Comp : public Component
{
public:
    Triangle1Comp();
    
    void Draw(const RenderInfo& RenderInfo) override;

    RenderPipeline pipeline;

    MeshVertexBuffer buffer;
};

Triangle1Comp::Triangle1Comp() : buffer(sizeof(TriangleVertex) * vertices.size(), vertices.data())
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



void Triangle1::Init()
{
    Level::Init();

    {
        auto obj = NewObject();
        obj->Attach(NewSPtr<Triangle1Comp>());
    }
    
}

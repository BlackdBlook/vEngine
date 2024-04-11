#include "DrawCube.h"

#include <array>
#include <chrono>
#include <stdexcept>

#include "Engine/vEngine.h"
#include "Engine/Core/Component/Component.h"
#include "Engine/Core/FrameInfo/RenderInfo.h"
#include "Engine/Core/MemoryBuffer/MeshVertexBuffer.h"
#include "Engine/Core/Object/Object.h"
#include "Engine/Core/ShaderModule/DescriptorHelper.h"
#include "Engine/Core/ShaderModule/PipelineShader.h"
#include "Engine/Core/UniformBuffer/UniformBuffer.h"
#include "Meshs/Box/BoxVertices.h"



namespace
{
    struct UniformBufferObject
    {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };
    
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
            attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex, Pos);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(Vertex, Normal);

            attributeDescriptions[2].binding = 0;
            attributeDescriptions[2].location = 2;
            attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[2].offset = offsetof(Vertex, TexCoords);
            return attributeDescriptions;
        }
    };
    
    class RenderPipelineInfo2 : public RenderPipelineInfo
    {
    public:
        VkDescriptorSetLayout descriptorSetLayout;
        VkPipelineLayout PipelineLayout() override;
        VkPipelineVertexInputStateCreateInfo* PipelineVertexInputStateCreateInfo() override;
    };
}

class Cube : public Component
{
    RenderPipeline pipeline;
    
    MeshVertexBuffer buffer;

    DescriptorHelper descriptor;
    
    VkDescriptorSetLayout layout;

public:
    Cube();
    ~Cube() override;

    void Update(float DeltaTime) override;
    
    void Draw(const RenderInfo& RenderInfo) override;
};

VkPipelineLayout RenderPipelineInfo2::PipelineLayout()
{
    //存储pipeline layout，用来指定创建管线时的uniform值
    VkPipelineLayout pipelineLayout;
    // VkDescriptorSetLayoutBinding uboLayoutBinding{};
    // uboLayoutBinding.binding = 0;
    // uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    // uboLayoutBinding.descriptorCount = 1;
    // uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    //
    // VkDescriptorSetLayoutCreateInfo layoutInfo{};
    // layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    // layoutInfo.bindingCount = 1;
    // layoutInfo.pBindings = &uboLayoutBinding;
    //
    // if (vkCreateDescriptorSetLayout(GDevice, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
    // {
    //     throw std::runtime_error("failed to create descriptor set layout!");
    // }

    //创建pipeline layout，指定shader uniform值
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1; // Optional
    pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout; // Optional


    if (vkCreatePipelineLayout(GDevice, &pipelineLayoutInfo,
        nullptr, &pipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create pipeline layout!");
    }
    return pipelineLayout;
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
 
    if (vkCreateDescriptorSetLayout(GDevice, &layoutInfo, nullptr, &ans) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }

    return ans;
}


Cube::Cube() : buffer(sizeof(BoxVertices), BoxVertices)
{
    layout = createDescriptorSetLayout();

    std::vector<UniformBuffer> ubuffers;
    
    ubuffers.emplace_back();
    
    ubuffers[0].Init(sizeof(UniformBufferObject), "Camera", 0);
    
    descriptor.createDescriptorSets(layout);
    descriptor.BindMemoryBuffer(std::move(ubuffers));

    
    RenderPipelineInfo2 info;
    info.descriptorSetLayout = layout;
    info.VertShaderName = "DrawCube";
    info.FragShaderName = "DrawCube";
    pipeline.Init(&info);
    
}

Cube::~Cube()
{
    vkDestroyDescriptorSetLayout(GDevice, layout, nullptr);
}

static auto startTime = std::chrono::high_resolution_clock::now();

void Cube::Update(float DeltaTime)
{
    Component::Update(DeltaTime);



    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
    
    UniformBufferObject ubo{};

    MAT4(m);

    // 平移
    m = glm::translate(m, VEC3_ZERO);

    // 旋转
    glm::quat quat = VEC3_ZERO;
    // // quat *= glm::angleAxis(0, glm::vec3{0,0,1});
    quat *= glm::angleAxis(time, glm::vec3{0,1,0});
    quat *= glm::angleAxis(time, glm::vec3{1,0,0});
    glm::mat4 rotationMatrix = glm::mat4_cast(quat);
    m *= rotationMatrix;
    //
    // // 缩放
    // m = glm::scale(m, glm::vec3{1,1,1});

    ubo.model = m;
    
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    ubo.proj = glm::perspective(glm::radians(90.0f), Engine::ins->WindowX / (float)Engine::ins->WindowY, 0.1f, 100.0f);
    
    ubo.proj[1][1] *= -1;

    descriptor.buffers[0].UpdateBuffer(&ubo, sizeof(ubo));
}

void Cube::Draw(const RenderInfo& RenderInfo)
{
    Component::Draw(RenderInfo);

    pipeline.CmdBind(RenderInfo.CommmandBuffer);

    buffer.CmdBind(RenderInfo.CommmandBuffer);

    vkCmdBindDescriptorSets(RenderInfo.CommmandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
        pipeline.layout, 0, 1,
        &descriptor.descriptorSets[Engine::ins->GetCurrentFrame()],
        0, nullptr);


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

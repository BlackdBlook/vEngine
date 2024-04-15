#include "DrawCubeV2.h"

#include <array>
#include <chrono>
#include <stdexcept>

#include "Engine/vEngine.h"
#include "Engine/Core/Component/Component.h"
#include "Engine/Core/FrameInfo/RenderInfo.h"
#include "Engine/Core/Material/Material.h"
#include "Engine/Core/Material/MaterialRenderPipeline.h"
#include "Engine/Core/MemoryBuffer/MeshVertexBuffer.h"
#include "Engine/Core/Object/Object.h"
#include "Engine/Core/ShaderModule/DescriptorHelper.h"
#include "Engine/Core/ShaderModule/PipelineShader.h"
#include "Engine/Core/ShaderModule/Shader.h"
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


    class Cube : public Component
    {
        Material material;   

        MeshVertexBuffer buffer;

    public:
        Cube();
        ~Cube() override;

        void Update(float DeltaTime) override;

        void Draw(const RenderInfo& RenderInfo) override;
    };
}

Cube::Cube() : material("DrawCube", ShaderCodeType::HLSL), buffer(sizeof(BoxVertices), BoxVertices)
{

}

Cube::~Cube()
{

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
    quat *= glm::angleAxis(time, glm::vec3{0, -1, 0});
    // quat *= glm::angleAxis(time, glm::vec3{1, 0, 0});
    glm::mat4 rotationMatrix = glm::mat4_cast(quat);
    m *= rotationMatrix;
    //
    // // 缩放
    // m = glm::scale(m, glm::vec3{1,1,1});

    ubo.model = m;

    ubo.view = glm::lookAt(glm::vec3(0.0f, 1.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    ubo.proj = glm::perspective(glm::radians(90.0f), Engine::ins->WindowX / (float)Engine::ins->WindowY, 0.1f, 100.0f);
    
    material.SetUniformData(0, (uint8*)&ubo, sizeof(ubo));
}

void Cube::Draw(const RenderInfo& RenderInfo)
{
    Component::Draw(RenderInfo);

    material.Draw(RenderInfo);

    buffer.CmdBind(RenderInfo.CommmandBuffer);
    
    vkCmdDraw(RenderInfo.CommmandBuffer, sizeof(BoxVertices) / 8, 1, 0, 0);
}

void DrawCubeV2::Init()
{
    Level::Init();

    {
        auto obj = NewObject();
        obj->Attach(NewSPtr<Cube>());
    }
}

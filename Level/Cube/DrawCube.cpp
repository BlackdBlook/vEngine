#include "DrawCube.h"

#include <array>
#include <chrono>
#include <SDL_timer.h>
#include <stdexcept>

#include "Engine/vEngine.h"
#include "Engine/Core/Component/Component.h"
#include "Engine/Core/FrameInfo/FrameRenderInfo.h"
#include "Engine/Core/Material/Material.h"
#include "Engine/Core/Material/MaterialRenderPipeline.h"
#include "Engine/Core/MemoryBuffer/MeshVertexBuffer.h"
#include "Engine/Core/Object/Object.h"
#include "Engine/Core/ShaderModule/DescriptorHelper.h"
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

        bool Flag = false;
        Cube();
        ~Cube() override;

        void Update(float DeltaTime) override;

        void Draw(FrameRenderInfo& RenderInfo) override;
    };
}

Cube::Cube() : material("DrawCube", ShaderCodeType::HLSL), buffer(sizeof(BoxVertices), BoxVertices)
{
    UniformBufferObject ubo{};
    
    ubo.view = glm::lookAt(glm::vec3(0.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    ubo.proj = glm::perspective(glm::radians(90.0f), Engine::ins->WindowX / (float)Engine::ins->WindowY, 1.f, 10.0f);

    material.SetAllUniformData("ubo.model", MAT4());
    material.SetAllUniformData("ubo.u_View", ubo.view);
    material.SetAllUniformData("ubo.u_Projection", ubo.proj);
}

Cube::~Cube()
{

}

void Cube::Update(float DeltaTime)
{
    Component::Update(DeltaTime);
    
    MAT4(m);

    // 平移
    m = glm::translate(m, ParentObject->GetPos());
    
    //
    // // 旋转
    glm::quat quat = VEC3_ZERO;
    // // quat *= glm::angleAxis(0, glm::vec3{0,0,1});
    quat *= glm::angleAxis((float)ImGui::GetTime(), glm::vec3{1, 0, 0});
    // quat *= glm::angleAxis(time, glm::vec3{1, 0, 0});
    glm::mat4 rotationMatrix = glm::mat4_cast(quat);
    m *= rotationMatrix;
    // //
    // // // 缩放
    // // m = glm::scale(m, glm::vec3{1,1,1});

    
    // material.SetCurrentUniformData(0, (uint8*)&ubo, sizeof(ubo));
    // material.SetCurrentUniformData("type.ubo", (uint8*)&ubo, sizeof(ubo));
    material.SetCurrentUniformData("ubo.model", m);

}

void Cube::Draw(FrameRenderInfo& RenderInfo)
{
    Component::Draw(RenderInfo);

    material.Draw(RenderInfo);

    buffer.CmdBind(RenderInfo.CommmandBuffer);
    
    vkCmdDraw(RenderInfo.CommmandBuffer, sizeof(BoxVertices) / 8, 1, 0, 0);
}

void DrawCube::Init()
{
    Level::Init();

    {
        auto obj = NewObject();
        obj->Attach(NewSPtr<Cube>());
        obj->SetPos(glm::vec3{2,0,0});
    }

    {
        auto obj = NewObject();
        auto cube = NewSPtr<Cube>();
        obj->Attach(cube);
        cube->Flag = true;
    }

    {
        auto obj = NewObject();
        obj->Attach(NewSPtr<Cube>());
        obj->SetPos(glm::vec3{-2,0,0});
    }
}
// LevelRegister(DrawCube);
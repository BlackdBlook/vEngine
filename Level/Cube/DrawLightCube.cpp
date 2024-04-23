#include "DrawLightCube.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Engine/vEngine.h"
#include "Engine/Core/Camera/Camera.h"
#include "Engine/Core/Component/Component.h"
#include "Engine/Core/GlobalUniformBuffer/GlobalUniformBufferManager.h"
#include "Engine/Core/Material/Material.h"
#include "Engine/Core/Object/Object.h"
#include "Engine/Core/Texture2D/Texture2D.h"
#include "Meshs/Box/BoxVertices.h"
#include "Meshs/Box/CubeComponent.h"


namespace
{
    class Light : public Component
    {
    public:
        
    };

    class AutoRot : public Component
    {
    public:
        void Update(float DeltaTime) override
        {
            Parent->AddRot(glm::vec3{
                1
                ,1
                ,1});
        }
    };

    class AutoMov : public Component
    {
    public:
        void Update(float DeltaTime) override
        {
            Parent->SetPos(glm::vec3{glm::sin(glfwGetTime()) * 2,0,glm::cos(glfwGetTime()) * 2});
        }
    };
}

void DrawLightCube::Init()
{
    Level::Init();

    Camera::GetCamera()->SetPos(glm::vec3(0, 0, 3));
    {
        auto light = NewObject();
        light->CreateAttach<Light>();
        light->CreateAttach<AutoMov>();
        light->CreateAttach<CubeComponent>("PointLightCube");
        light->SetScale(glm::vec3{0.1});
    }

    // for(int i = 0;i < 100;i++)
    {
        auto obj = NewObject();
        obj->CreateAttach<AutoRot>();
        auto cube = obj->CreateAttach<CubeComponent>("PointLightCube");
        cube->material->SetTexture("texture0", "container2.png");
    }

}
LevelRegister(DrawLightCube);

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
#include "Engine/Runtime/Component/CubeComponent.h"
#include "Meshs/Box/BoxVertices.h"



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
        Material* material;
    public:
        AutoMov(Material* material)
        {
            Component();
            this->material = material;
        }
        void Update(float DeltaTime) override
        {
            Parent->SetPos(glm::vec3{glm::sin(glfwGetTime()) * 2,0,glm::cos(glfwGetTime()) * 2});
            material->SetCurrentUniformData("pointLight.position", Parent->GetPos());
        }
    };


}

void DrawLightCube::Init()
{
    Level::Init();

    Camera::GetCamera()->SetPos(glm::vec3(0, 0, 3));
    Material* m;

    {
        auto obj = NewObject();
        // obj->CreateAttach<AutoRot>();
        auto cube = obj->CreateAttach<CubeComponent>("PointLightCube");
        m = cube->material.get();
        m->SetTexture("texture0", "container2.png");
        m->SetAllUniformData("material.shininess", 1.f);
        m->SetAllUniformData("pointLight.constant", 1.f);
        m->SetAllUniformData("pointLight.linear", 0.09f);
        m->SetAllUniformData("pointLight.quadratic", 0.032f);
        m->SetAllUniformData("pointLight.strength", 1.f);
        m->SetAllUniformData("pointLight.position", glm::vec3{1, 1, 1});
        m->SetAllUniformData("pointLight.color", glm::vec3{1, 1, 1});
    }
    
    {
        auto light = NewObject();
        light->CreateAttach<Light>();
        light->CreateAttach<AutoMov>(m);
        light->CreateAttach<CubeComponent>("DrawTexCube");
        light->SetScale(glm::vec3{0.1});
    }


}
LevelRegister(DrawLightCube);

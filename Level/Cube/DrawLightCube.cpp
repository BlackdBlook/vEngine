#include "DrawLightCube.h"

#include <SDL_timer.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Component/Common/BasicMove/BasicCameraMove.h"
#include "Component/Common/EscExit/PressEscExit.h"
#include "Component/Envriment/Skybox/Skybox.h"
#include "Engine/vEngine.h"
#include "Engine/Core/Camera/Camera.h"
#include "Engine/Core/Component/Component.h"
#include "Engine/Core/Material/Material.h"
#include "Engine/Core/Object/Object.h"
#include "Engine/Core/Texture/TextureCube/TextureCube.h"
#include "Engine/Runtime/Component/CubeComponent.h"
#include "Meshs/Box/BoxVertices.h"


class TextureCube;

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
            ParentObject->AddRot(glm::vec3{
                1
                ,1
                ,1});
        }
    };
    
    class UIDemo :public Component
    {
        Material* material;
        float LightStrength = 1.f;
    public:
        bool Turn = false;
        UIDemo(Material* m):material(m)
        {
            Component();
        }
        virtual void Update(float DeltaTime) override;
    };

    class AutoMov : public Component
    {
        Material* material;
        SPtr<UIDemo> ui;
    public:
        AutoMov(Material* material)
        {
            Component();
            this->material = material;
        }

        void OnAttached() override
        {
            ui = ParentObject->FindComponent<UIDemo>();
        }
        
        void Update(float DeltaTime) override
        {
            if(ui && ui->Turn)
            {
                ParentObject->SetPos(glm::vec3{glm::sin(ImGui::GetTime()) * 2,0,glm::cos(ImGui::GetTime()) * 2});
                material->SetCurrentUniformData("pointLight.position", ParentObject->GetPos());
            }
        }
    };


    void UIDemo::Update(float DeltaTime)
    {
        ImGui::Begin("UIDemo");
        ImGui::SetWindowPos(ImVec2(0, 0));
        ImGui::Text("Hello, world!");
        // ImGui::SliderFloat("float", &ParentObject->GetPos().x, 0.0f, 1.0f);
        auto pos = ParentObject->GetPos();
        float p[3] = {pos.x, pos.y, pos.z};
        ImGui::DragFloat3("LightPos", p, 0.001f);
        ImGui::DragFloat("LightStrength", &LightStrength, 0.001f);
        ImGui::Checkbox("Turn", &Turn);
        ImGui::End();

        ParentObject->SetPos(glm::vec3{p[0], p[1], p[2]});
        material->SetCurrentUniformData("pointLight.position", ParentObject->GetPos());
        material->SetCurrentUniformData("pointLight.strength", LightStrength);
    }
}

void DrawLightCube::Init()
{
    Level::Init();

    Camera::GetCamera()->SetPos(glm::vec3(0, 0, 3));
    

    {
        auto obj = NewObject();
        // obj->CreateAttach<AutoRot>();
        auto cube = obj->CreateSetRootComponent<CubeComponent>("PointLightCube");
        m = cube->material.get();
        m->SetTexture("texture0", "container2.png");
        m->SetTexture("texture1", "container2_specular.png");
        m->SetAllUniformData("material.shininess", 64.f);
        m->SetAllUniformData("pointLight.constant", 1.f);
        m->SetAllUniformData("pointLight.linear", 0.09f);
        m->SetAllUniformData("pointLight.quadratic", 0.032f);
        m->SetAllUniformData("pointLight.strength", 1.f);
        m->SetAllUniformData("pointLight.position", glm::vec3{1, 1, 1});
        m->SetAllUniformData("pointLight.color", glm::vec3{1, 1, 1});
    }
    
    {
        auto light = NewObject();
        light->CreateSetRootComponent<CubeComponent>("DrawTexCube");
        light->CreateAttach<UIDemo>(m);
        light->CreateAttach<Light>();
        light->CreateAttach<AutoMov>(m);
        // light->CreateAttach<CubeComponent>("DrawTexCube");
        light->SetScale(glm::vec3{0.1});
        light->SetPos(glm::vec3{0,0,2});
        light->CreateAttach<BasicCameraMove>();
        light->CreateAttach<PressEscExit>();

    }

    {
        auto skyBox = NewObject();
        skyBox->CreateSetRootComponent<Skybox>("CommonSkyBox");
    }
}
LevelRegister(DrawLightCube);

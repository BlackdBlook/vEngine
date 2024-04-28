#include "BasicCameraMove.h"

#include "Engine/vEngine.h"
#include "Engine/Core/Camera/Camera.h"
#include "Engine/Core/GlobalUniformBuffer/GlobalUniformBufferManager.h"
#include "Engine/SubSystem/InputSystem/InputSysten.h"

BasicCameraMove::BasicCameraMove()
{
    cam = Camera::GetCamera();
    window = Engine::ins->GetWindow();
    input = InputSystem::GetInstance();
}

void BasicCameraMove::Update(float DeltaTime)
{
    Component::Update(DeltaTime);

    constexpr float MOVE_SPEED = 5.f;
    glm::vec3 pos = cam->GetPos();
    const float MoveSpeed = MOVE_SPEED * DeltaTime;
    bool flag = false;
    auto r = cam->GetRight();
    r.y = 0;
    r = glm::normalize(r);
    auto u = glm::vec3{0, 1, 0};
    u = glm::normalize(u);
    auto f = glm::cross(r,u);
    f = glm::normalize(f);
        
    if (input->GetKeyDown(KeyBoardKey::KEY_W))
    {
        pos+= MoveSpeed * f;
        flag = true;
    }

    if (input->GetKeyDown(KeyBoardKey::KEY_S))    {
        pos+= -MoveSpeed * f;
        flag = true;
    }

    if (input->GetKeyDown(KeyBoardKey::KEY_A))    {
        pos += MoveSpeed * r;
        flag = true;
    }

    if (input->GetKeyDown(KeyBoardKey::KEY_D))    {
        pos += -MoveSpeed * r;
        flag = true;
    }
    

    if (input->GetKeyDown(KeyBoardKey::KEY_E))    {
        pos+= MoveSpeed * u;
        flag = true;
    }

    if (input->GetKeyDown(KeyBoardKey::KEY_Q))    {
        pos+= -MoveSpeed * u;
        flag = true;
    }
    if(flag)
    {
        cam->SetPos(pos);
    }
       
    InputSystem* input = InputSystem::GetInstance();

    if(input->GetMouseButtonDown(true))
    {
        if(window == nullptr)
        {
            window = Engine::ins->GetWindow();
            tempX = input->MousePositionX;
            tempY = input->MousePositionY;
            return;
        }
        
            
        glm::vec3 inputR ={ (input->MousePositionY - tempY) * -0.2,  (input->MousePositionX - tempX) * -0.2, 0};

        if(inputR.x != 0)
        {
            constexpr float minP = -80.0f;
            constexpr float maxP = 80.0f;
            const float tX = rot.x + inputR.x;
            if(tX < minP || tX > maxP)
            {
                inputR.x = 0;
            }
        }
            
        rot += inputR;
        rot.z = 0;

        cam->SetRot(rot);

        GlobalUniformBufferManager::Get()->UpdateCameraData();

        tempX = input->MousePositionX;
        tempY = input->MousePositionY;
    }else
    {
        if(window)
        {
            window = nullptr;
        }
    }
}

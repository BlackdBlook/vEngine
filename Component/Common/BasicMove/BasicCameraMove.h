#pragma once
#include <SDL_video.h>

#include "Engine/Core/Component/Component.h"
#include "Engine/SubSystem/InputSystem/InputSysten.h"

class Camera;

class BasicCameraMove : public Component
{
    Camera* cam;
    SDL_Window* window;
    double tempX = 0, tempY = 0;
    glm::vec3 rot {0};
    InputSystem* input;

public:
    BasicCameraMove();
    void Update(float DeltaTime) override;
};

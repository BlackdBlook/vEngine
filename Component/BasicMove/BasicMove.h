#pragma once
#include <GLFW/glfw3.h>

#include "Engine/Core/Component/Component.h"

class Camera;

class BasicMove : public Component
{
    Camera* cam;
    GLFWwindow* window;
    double tempX = 0, tempY = 0;
    glm::vec3 rot {0};
public:
    BasicMove();
    void Update(float DeltaTime) override;
};

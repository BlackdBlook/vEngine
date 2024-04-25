#pragma once
#include <GLFW/glfw3.h>

class InputSystem
{
public:
    void Init(GLFWwindow* window);

    bool GetMouseButtonDown(bool Right);

    static InputSystem* GetInstance();
private:

    bool leftMouseButtonDown = false;
    bool rightMouseButtonDown = false;

    static void MouseCallback(GLFWwindow* window, int button, int state, int mod);
    
    InputSystem();
};

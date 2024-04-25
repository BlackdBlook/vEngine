#include "InputSysten.h"
#include "Header.h"



void InputSystem::Init(GLFWwindow* window)
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetMouseButtonCallback(window, InputSystem::MouseCallback);
}

bool InputSystem::GetMouseButtonDown(bool Right)
{
    if(Right)
    {
        return rightMouseButtonDown;
    }else
    {
        return leftMouseButtonDown;
    }
}
 
InputSystem* InputSystem::GetInstance()
{ 
    static InputSystem* ans = new InputSystem();
    return ans; 
}

void InputSystem::MouseCallback(GLFWwindow* window, int button, int state, int mod)
{
    if(button == GLFW_MOUSE_BUTTON_LEFT)
    {
        GetInstance()->leftMouseButtonDown = (state==GLFW_PRESS);
        // LOG("MouseButtonLeft", GetInstance()->leftMouseButtonDown);
    }else if(button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        GetInstance()->rightMouseButtonDown = (state==GLFW_PRESS);
        // LOG("MouseButtonRight", GetInstance()->rightMouseButtonDown);
    }
}

InputSystem::InputSystem()
{
}

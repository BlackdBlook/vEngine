#include "InputSysten.h"

#include <SDL_keyboard.h>
#include <SDL_mouse.h>

#include "Header.h"
#include "LogPrinter/Log.h"


void InputSystem::Init(SDL_Window* window)
{

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

bool InputSystem::GetKeyDown(KeyBoardKey Key)
{
    const Uint8* state = SDL_GetKeyboardState(NULL);
    SDL_Scancode scancode = SDL_GetScancodeFromKey(static_cast<SDL_Keycode>(Key));
    return state[scancode];
}

void InputSystem::Update(float DeltaTime)
{
    Uint32 state = SDL_GetMouseState(&MousePositionX, &MousePositionY);
    
    leftMouseButtonDown = (state & SDL_BUTTON(SDL_BUTTON_LEFT));
    
    rightMouseButtonDown = (state & SDL_BUTTON(SDL_BUTTON_RIGHT));
}

InputSystem* InputSystem::GetInstance()
{ 
    static InputSystem* ans = new InputSystem();
    return ans; 
}

InputSystem::InputSystem()
{
}

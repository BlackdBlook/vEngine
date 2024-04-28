#pragma once
#include <SDL_keycode.h>
#include <SDL_video.h>

#include "Engine/TypeDef.h"

enum class KeyBoard
{
    
};

enum class KeyBoardKey : int
{
    KEY_A = SDLK_a,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,
    KEY_0 = SDLK_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_SPACE = SDLK_SPACE,
    KEY_ENTER = SDLK_RETURN,
    KEY_BACKSPACE = SDLK_BACKSPACE,
    KEY_TAB = SDLK_TAB,
    KEY_ESCAPE = SDLK_ESCAPE,
    KEY_UP = SDLK_UP,
    KEY_DOWN = SDLK_DOWN,
    KEY_LEFT = SDLK_LEFT,
    KEY_RIGHT = SDLK_RIGHT,
    KEY_LSHIFT = SDLK_LSHIFT,
    KEY_RSHIFT = SDLK_RSHIFT,
    KEY_LCTRL = SDLK_LCTRL,
    KEY_RCTRL = SDLK_RCTRL,
    KEY_LALT = SDLK_LALT,
    KEY_RALT = SDLK_RALT
};

class InputSystem
{
public:
    void Init(SDL_Window* window);

    bool GetMouseButtonDown(bool Right);
    bool GetKeyDown(KeyBoardKey Key);

    uint32 MousePositionX = 0;
    uint32 MousePositionY = 0;

    void Update();

    static InputSystem* GetInstance();
private:

    bool leftMouseButtonDown = false;
    bool rightMouseButtonDown = false;
    
    InputSystem();
};

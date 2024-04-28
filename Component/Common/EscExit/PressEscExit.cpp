#include "PressEscExit.h"

#include <SDL_events.h>

#include "Engine/vEngine.h"
#include "Engine/SubSystem/InputSystem/InputSysten.h"

void PressEscExit::Update(float DeltaTime)
{
    Component::Update(DeltaTime);
    if(InputSystem::GetInstance()->GetKeyDown(KeyBoardKey::KEY_ESCAPE))
    {
        SDL_PushEvent(new SDL_Event{SDL_QUIT});
    }
}

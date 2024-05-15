#pragma once
#include "RenderCommand.h"
#include "Engine/Toolkit/Container/Queue.h"

class RenderCommandQueue
{
    TQueue<RenderCommand> RenderCommands;
public:
    void PushRenderCommand();

    static RenderCommandQueue* Instance;
};

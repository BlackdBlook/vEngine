#pragma once
#include "RenderCommand.h"
#include "Engine/Toolkit/Container/Queue.h"

class RenderCommandQueue
{

    TQueue<RenderCommand> commands;
    
public:

    void PushRenderCommand(RenderCommand&& NewCommand);
    
};

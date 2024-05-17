#pragma once
#include <map>

#include "RenderCommand.h"
#include "Engine/Core/FrameInfo/FrameInfo.h"
#include "Engine/Toolkit/Container/Queue.h"

class RenderCommandQueue
{

public:
    
    std::map<float, SceneComponentRenderInfo*> TranslucentOrderInfo;
    std::map<float, SceneComponentRenderInfo*> OpaqueOrderInfo;
    SceneComponentRenderInfo* SkyRenderInfo = nullptr;

    void PushRenderCommand(FrameInfo& NewCommand);
    
};

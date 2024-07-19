#pragma once
#include <vulkan/vulkan_core.h>

#include "Engine/TypeDef.h"
#include "Engine/Toolkit/Container/Queue.h"

class SceneComponentRenderInfo;

class FrameRenderInfo
{
public:
    const VkCommandBuffer CommmandBuffer;
    const VkExtent2D extent;
    TQueue<SceneComponentRenderInfo*> SceneComponentRenderInfos;
};

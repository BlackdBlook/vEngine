#pragma once
#include <vulkan/vulkan_core.h>

#include "Engine/TypeDef.h"

class RenderInfo
{
public:
    VkCommandBuffer CommmandBuffer;
    VkExtent2D extent;
};

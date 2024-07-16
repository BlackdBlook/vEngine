#pragma once
#include "Engine/Core/UniformBuffer/GlobalUniformBuffer/GlobalUniformBufferManager.h"

class RenderCommandQueue;

class IRendering
{
public:

    virtual VkRenderPass GetOpaqueRenderPass() = 0;
    virtual VkRenderPass GetTranslucentRenderPass() = 0;
    
    virtual void FrameRender(RenderCommandQueue& queue, VkCommandBuffer cmd) = 0;

    virtual ~IRendering();
};

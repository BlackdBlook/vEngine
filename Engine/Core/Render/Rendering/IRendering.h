#pragma once
#include "Engine/Core/UniformBuffer/GlobalUniformBuffer/GlobalUniformBufferManager.h"

class RenderCommandQueue;

class IRendering
{
public:

    using RenderPassContainer = std::unordered_map<Container::Name, VkRenderPass*>;
    virtual const RenderPassContainer& GetNamedRenderPasses() = 0;

    using FramebufferContainer = std::unordered_map<Container::Name, std::vector<VkFramebuffer>*>;
    virtual const FramebufferContainer& GetNamedFrameBuffers() = 0;
    
    virtual VkImage GetSceneColor(uint32 FrameIndex) = 0;
    virtual VkImage GetSceneDepth(uint32 FrameIndex) = 0;
    
    virtual void FrameRender(RenderCommandQueue& queue, VkCommandBuffer cmd) = 0;

    virtual ~IRendering();
};

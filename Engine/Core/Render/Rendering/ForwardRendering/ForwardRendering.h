#pragma once
#include "Engine/Core/Render/RenderCommandQueue.h"
#include "Engine/Core/Render/Rendering/IRendering.h"

class ForwardRendering : public IRendering
{
public:
    VkRenderPass OpaqueRenderPass;
    std::vector<VkFramebuffer> OpaqueRenderPassFramebuffers;
    VkRenderPass TranslucentRenderPass;
    std::vector<VkFramebuffer> TranslucentRenderPassFramebuffers;

    VkImage depthImage[MAX_FRAMES_IN_FLIGHTS];
    VkDeviceMemory depthImageMemory[MAX_FRAMES_IN_FLIGHTS];
    VkImageView depthImageView[MAX_FRAMES_IN_FLIGHTS];

    VkImage colorImage[MAX_FRAMES_IN_FLIGHTS];
    VkDeviceMemory colorImageMemory[MAX_FRAMES_IN_FLIGHTS];
    VkImageView colorImageView[MAX_FRAMES_IN_FLIGHTS];
    
    ForwardRendering();
    void createDepthResources();
    void ReleaseDepthResources();

    void createColorResources();
    void ReleaseColorResources();
    
    void createOpaqueRenderPass(); 
    void createTranslucentRenderPass();

    void FrameRender(RenderCommandQueue& queue, VkCommandBuffer cmd) override;

    ~ForwardRendering() override;
    VkRenderPass GetOpaqueRenderPass() override;
    VkRenderPass GetTranslucentRenderPass() override;
};

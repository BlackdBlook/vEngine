#pragma once
#include "Engine/Core/Render/RenderCommandQueue.h"
#include "Engine/Core/Render/Rendering/IRendering.h"

struct FrameBufferData;

class ForwardRendering : public IRendering
{
public:
    VkRenderPass OpaqueRenderPass;
    std::vector<VkFramebuffer> OpaqueRenderPassFramebuffers;
    
    VkRenderPass TranslucentRenderPass;
    std::vector<VkFramebuffer> TranslucentRenderPassFramebuffers;

    VkRenderPass SkyRenderPass;
    std::vector<VkFramebuffer> SkyRenderPassFramebuffers;

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
    void createSkyRenderPass();

    void FrameRender(RenderCommandQueue& queue, VkCommandBuffer cmd) override;

    void Attach2Read(VkCommandBuffer cmd, VkImage Image);
    void Read2Attach(VkCommandBuffer cmd, VkImage Image);

    ~ForwardRendering() override;

    VkImage GetSceneColor(uint32 FrameIndex) override;
    VkImage GetSceneDepth(uint32 FrameIndex) override;
    const RenderPassContainer& GetNamedRenderPasses() override;
    const FramebufferContainer& GetNamedFrameBuffers() override;
};

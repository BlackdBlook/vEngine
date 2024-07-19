#pragma once
#include "Engine/Core/Material/Material.h"

class RenderPostProcessing
{
public:
    VkRenderPass PostProcessRenderPass = VK_NULL_HANDLE;
    VkFramebuffer PostProcessFrameBuffer[MAX_FRAMES_IN_FLIGHTS] = {VK_NULL_HANDLE};
    
    void CreateRenderPass();
    void ReleaseRenderPass();

    SPtr<Material> mat;
    void Update();
    void CmdRender(VkCommandBuffer cmd, uint32 FrameIndex);
    
    RenderPostProcessing();
    ~RenderPostProcessing();
};

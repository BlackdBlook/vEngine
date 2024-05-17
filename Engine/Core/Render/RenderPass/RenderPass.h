#pragma once
#include "Engine/Core/UniformBuffer/GlobalUniformBuffer/GlobalUniformBufferManager.h"

class RenderPass
{
    VkRenderPass renderPass;
public:

    RenderPass(
        VkImageView ColorAttachment,
        VkImageView DepthAttachment);
    
    void cmdBeginRenderPass(VkCommandBuffer cmd);
};

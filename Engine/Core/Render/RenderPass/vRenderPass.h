#pragma once
#include "Engine/Core/UniformBuffer/GlobalUniformBuffer/GlobalUniformBufferManager.h"

class vRenderPass
{
    VkRenderPass renderPass;
public:

    vRenderPass(
        VkImageView ColorAttachment,
        VkImageView DepthAttachment);
    
    void cmdBeginRenderPass(VkCommandBuffer cmd);
};

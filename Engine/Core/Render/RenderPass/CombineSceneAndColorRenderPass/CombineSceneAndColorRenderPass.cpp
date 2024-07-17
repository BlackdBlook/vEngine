#include "CombineSceneAndColorRenderPass.h"

#include <array>

#include "Engine/VulkanHelper/VkHelper.h"


CombineSceneAndColorRenderPass::CombineSceneAndColorRenderPass(VkImageView SceneColor, VkImageView UIColor)
{
    CreateRenderPass();
}

void CombineSceneAndColorRenderPass::RenderToSurface()
{
    
}

void CombineSceneAndColorRenderPass::CreateRenderPass()
{
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = VkHelperInstance->SwapSurfaceFormat.format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

    // 不清除颜色
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;


    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    
    std::array<VkAttachmentDescription, 1> attachments = {colorAttachment};

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    // renderPassInfo.dependencyCount = 1;
    // renderPassInfo.pDependencies = &dependency;

    
    if (vkCreateRenderPass(VkHelperInstance->Device, &renderPassInfo, nullptr, &RenderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }
}

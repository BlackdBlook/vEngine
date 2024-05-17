#include "RenderPass.h"

#include <array>

#include "Engine/vEngine.h"
#include "Engine/VulkanHelper/VkHelper.h"

RenderPass::RenderPass(VkImageView ColorAttachment, VkImageView DepthAttachment)
{
    bool EnableColorAttachment = (ColorAttachment != VK_NULL_HANDLE);
    bool EnableDepthAttachment = (DepthAttachment != VK_NULL_HANDLE);

    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = VkHelperInstance->SwapSurfaceFormat.format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

    // 不清除颜色
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;


    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription depthAttachment = {};
    depthAttachment.format = VkHelperInstance->findDepthFormat();
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    // 清除深度
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef = {};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    
    std::vector<VkAttachmentDescription> attachments;
    if(EnableColorAttachment)
    {
        attachments.push_back(colorAttachment);
    }
    if(EnableDepthAttachment)
    {
        attachments.push_back(depthAttachment);
    }

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = EnableColorAttachment ? &colorAttachmentRef : nullptr; 
    subpass.pDepthStencilAttachment = EnableDepthAttachment ? &depthAttachmentRef : nullptr;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    // renderPassInfo.dependencyCount = 1;
    // renderPassInfo.pDependencies = &dependency;

    
    if (vkCreateRenderPass(GDevice, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }
}

void RenderPass::cmdBeginRenderPass(VkCommandBuffer cmd)
{
    
}

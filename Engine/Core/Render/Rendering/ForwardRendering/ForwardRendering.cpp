#include "ForwardRendering.h"

#include <array>

#include "Engine/vEngine.h"
#include "Engine/Core/Material/Material.h"
#include "Engine/Core/Render/RenderCommandQueue.h"
#include "Engine/Core/Render/SceneComponentRenderInfo.h"
#include "Engine/Core/Render/FrameBuffer/FrameBufferData.h"

ForwardRendering::ForwardRendering()
{
    createColorResources();
    createDepthResources();
    createOpaqueRenderPass();
    createTranslucentRenderPass();
}

void ForwardRendering::createDepthResources()
{
    VkFormat depthFormat = VkHelperInstance->findDepthAttachmentFormat();

    auto cmd = VkHelperInstance->BeginSingleTimeCommands();
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHTS; i++)
    {
        VkHelperInstance->createImage(
            VkHelperInstance->swapChainExtent.width,
            VkHelperInstance->swapChainExtent.height,
            depthFormat, 0, VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage[i],
            depthImageMemory[i]
            );
    
        depthImageView[i] = VkHelperInstance->createImageView(depthImage[i], depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, VK_IMAGE_VIEW_TYPE_2D);
    

        VkHelperInstance->transitionImageLayout(cmd, depthImage[i], depthFormat, VK_IMAGE_LAYOUT_UNDEFINED,
                VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

    }
    VkHelperInstance->EndSingleTimeCommands(cmd);
}

void ForwardRendering::ReleaseDepthResources()
{
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHTS; i++)
    {
        vkDestroyImageView(GDevice, depthImageView[i], GAllocatorCallback);
        vkDestroyImage(GDevice, depthImage[i], GAllocatorCallback);
        vkFreeMemory(GDevice, depthImageMemory[i], GAllocatorCallback);
    }
}

void ForwardRendering::createColorResources()
{
    VkFormat colorFormat = VK_FORMAT_R8G8B8A8_SRGB;

    auto cmd = VkHelperInstance->BeginSingleTimeCommands();
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHTS; i++)
    {
        VkHelperInstance->createImage(
            VkHelperInstance->swapChainExtent.width,
            VkHelperInstance->swapChainExtent.height,
            colorFormat, 0, VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, colorImage[i],
            colorImageMemory[i]
            );
    
        colorImageView[i] = VkHelperInstance->createImageView(colorImage[i], colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_VIEW_TYPE_2D);
    

        // VkHelperInstance->transitionImageLayout(cmd, colorImage[i], colorFormat,
        //          VK_IMAGE_LAYOUT_UNDEFINED,
        //          VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.image = colorImage[i];
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        VkPipelineStageFlags srcStage;
        VkPipelineStageFlags dstStage;

        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

        vkCmdPipelineBarrier(
            cmd,
            srcStage,
            dstStage,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier
        );

    }
    VkHelperInstance->EndSingleTimeCommands(cmd);
}

void ForwardRendering::ReleaseColorResources()
{
    for(int i = 0; i < MAX_FRAMES_IN_FLIGHTS; i++)
    {
        vkDestroyImageView(GDevice, colorImageView[i], GAllocatorCallback);
        vkDestroyImage(GDevice, colorImage[i], GAllocatorCallback);
        vkFreeMemory(GDevice, colorImageMemory[i], GAllocatorCallback);
    }
}

void ForwardRendering::createOpaqueRenderPass()
{
    
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = VkHelperInstance->SwapSurfaceFormat.format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

    // 不清除颜色
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;


    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription depthAttachment = {};
    depthAttachment.format = VkHelperInstance->findDepthAttachmentFormat();
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
    
    std::array<VkAttachmentDescription, 2> attachments = {colorAttachment, depthAttachment};

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    // renderPassInfo.dependencyCount = 1;
    // renderPassInfo.pDependencies = &dependency;

    
    if (vkCreateRenderPass(VkHelperInstance->Device, &renderPassInfo, nullptr, &OpaqueRenderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }
    
    {
        OpaqueRenderPassFramebuffers.resize(VkHelperInstance->MainWindowData.ImageCount);
        for (size_t i = 0; i < VkHelperInstance->MainWindowData.ImageCount; i++) {
            std::array<VkImageView, 2> attachmentViews{
                colorImageView[i],
                depthImageView[i]
            };
    
            
            VkFramebufferCreateInfo framebufferInfo = {};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = OpaqueRenderPass;
            framebufferInfo.attachmentCount = (uint32)attachmentViews.size();
            framebufferInfo.pAttachments = attachmentViews.data();
            framebufferInfo.width = VkHelperInstance->swapChainExtent.width;
            framebufferInfo.height = VkHelperInstance->swapChainExtent.height;
            framebufferInfo.layers = 1;
    
            if (vkCreateFramebuffer(VkHelperInstance->Device, &framebufferInfo, nullptr, &OpaqueRenderPassFramebuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create framebuffer!");
            }
        }
    }
}

void ForwardRendering::createTranslucentRenderPass()
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

    
    if (vkCreateRenderPass(VkHelperInstance->Device, &renderPassInfo, nullptr, &TranslucentRenderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }
    
    {
        TranslucentRenderPassFramebuffers.resize(VkHelperInstance->MainWindowData.ImageCount);
        for (size_t i = 0; i < VkHelperInstance->MainWindowData.ImageCount; i++) {
            VkFramebufferCreateInfo framebufferInfo = {};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = TranslucentRenderPass;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = &VkHelperInstance->MainWindowData.Frames[i].BackbufferView;
            framebufferInfo.width = VkHelperInstance->swapChainExtent.width;
            framebufferInfo.height = VkHelperInstance->swapChainExtent.height;
            framebufferInfo.layers = 1;
    
            if (vkCreateFramebuffer(GDevice, &framebufferInfo, nullptr, &TranslucentRenderPassFramebuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create framebuffer!");
            }
        }
    }
}

void ForwardRendering::FrameRender(RenderCommandQueue& queue, VkCommandBuffer cmd)
{
    auto& vkHelper = *VkHelperInstance;
    ImGui_ImplVulkanH_Window* wd = &vkHelper.MainWindowData;
    uint32& FrameIndex = queue.FrameIndex;


    Read2Attach(cmd, colorImage[FrameIndex]);
    
    if(!queue.OpaqueOrderInfo.empty())
    {   // Opaque
        {
            std::array<VkClearValue, 2> clearValues = {};
            clearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
            clearValues[1].depthStencil = {1.0f, 0};
            
            VkRenderPassBeginInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            info.renderPass = OpaqueRenderPass;
            info.framebuffer = OpaqueRenderPassFramebuffers[FrameIndex];
            // info.framebuffer = fd->Framebuffer;
            info.renderArea.extent.width = wd->Width;
            info.renderArea.extent.height = wd->Height;
            info.clearValueCount = static_cast<uint32_t>(clearValues.size());
            info.pClearValues = clearValues.data();
        
            vkCmdBeginRenderPass(cmd, &info, VK_SUBPASS_CONTENTS_INLINE);
        }

        FrameRenderInfo RenderInfo {
            cmd,
            vkHelper.swapChainExtent,
            {}
        };
        
        if(queue.SkyRenderInfo != nullptr)
        {
            queue.SkyRenderInfo->material->Draw(RenderInfo);
            queue.SkyRenderInfo->VertexBuffer->CmdBind(cmd);
            vkCmdDraw(cmd, queue.SkyRenderInfo->VertexBuffer->GetVertexNumber(),
                1, 0, 0);
        }

        for(auto it = queue.OpaqueOrderInfo.begin(); it != queue.OpaqueOrderInfo.end(); ++it)
        {
            auto info = it->second;
            auto mt = info->material;
            mt->Draw(RenderInfo);
        
            info->VertexBuffer->CmdBind(cmd);

            vkCmdDraw(cmd, info->VertexBuffer->GetVertexNumber(),
                1, 0, 0);
        }

        // Submit command buffer
        vkCmdEndRenderPass(cmd);
    }

    if(!queue.TranslucentOrderInfo.empty())
    {
        // Translucent
        {
            VkRenderPassBeginInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            info.renderPass = TranslucentRenderPass;
            info.framebuffer = TranslucentRenderPassFramebuffers[FrameIndex];
            // info.framebuffer = fd->Framebuffer;
            info.renderArea.extent.width = wd->Width;
            info.renderArea.extent.height = wd->Height;
            info.clearValueCount = 0;
            info.pClearValues = nullptr;
        
            vkCmdBeginRenderPass(cmd, &info, VK_SUBPASS_CONTENTS_INLINE);
        }
        FrameRenderInfo RenderInfo {
            cmd,
            vkHelper.swapChainExtent,
            {}
        };

        for(auto it = queue.TranslucentOrderInfo.cbegin(); it != queue.TranslucentOrderInfo.cbegin(); ++it)
        {
            auto info = it->second;
            auto mt = info->material;
            mt->Draw(RenderInfo);
        
            info->VertexBuffer->CmdBind(cmd);

            vkCmdDraw(cmd, info->VertexBuffer->GetVertexNumber(),
                1, 0, 0);
        }

        // Submit command buffer
        vkCmdEndRenderPass(cmd);
    }
    Attach2Read(cmd, colorImage[FrameIndex]);
}

void ForwardRendering::Attach2Read(VkCommandBuffer cmd, VkImage Image)
{
    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.image = Image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags srcStage;
    VkPipelineStageFlags dstStage;

    barrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    srcStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

    vkCmdPipelineBarrier(
        cmd,
        srcStage,
        dstStage,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier
    );
}

void ForwardRendering::Read2Attach(VkCommandBuffer cmd, VkImage Image)
{
    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    barrier.image = Image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags srcStage;
    VkPipelineStageFlags dstStage;

    barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
    barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    srcStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    dstStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    vkCmdPipelineBarrier(
        cmd,
        srcStage,
        dstStage,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier
    );
}

ForwardRendering::~ForwardRendering()
{
    vkDestroyRenderPass(GDevice, OpaqueRenderPass, VkHelperInstance->Allocator);
    vkDestroyRenderPass(GDevice, TranslucentRenderPass, VkHelperInstance->Allocator);
    for(auto framebuffer : OpaqueRenderPassFramebuffers)
    {
        vkDestroyFramebuffer(GDevice, framebuffer, VkHelperInstance->Allocator);
    }
    for(auto framebuffer : TranslucentRenderPassFramebuffers)
    {
        vkDestroyFramebuffer(GDevice, framebuffer, VkHelperInstance->Allocator);
    }

    ReleaseColorResources();
    ReleaseDepthResources();
}

VkRenderPass ForwardRendering::GetOpaqueRenderPass()
{
    return OpaqueRenderPass;
}

VkRenderPass ForwardRendering::GetTranslucentRenderPass()
{
    return TranslucentRenderPass;
}

VkImageView ForwardRendering::GetSceneColor(uint32 FrameIndex)
{
    return colorImageView[FrameIndex];
}

VkImageView ForwardRendering::GetSceneDepth(uint32 FrameIndex)
{
    return depthImageView[FrameIndex];
}

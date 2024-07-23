#include "RenderPostProcessing.h"

#include <array>

#include "Engine/vEngine.h"
#include "Engine/Core/FrameInfo/FrameRenderInfo.h"
#include "Engine/Core/Texture/TextureInterface/ITexture.h"
#include "Engine/VulkanHelper/VkHelper.h"

void RenderPostProcessing::CreateRenderPass()
{
    
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = VkHelperInstance->SwapSurfaceFormat.format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
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

    
    if (vkCreateRenderPass(GDevice, &renderPassInfo,
        nullptr, &PostProcessRenderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }

    {
        ImGui_ImplVulkanH_Window* wd = &VkHelperInstance->MainWindowData;
        VkImageView attachment[1];
        VkFramebufferCreateInfo info;
        info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        info.renderPass = PostProcessRenderPass;
        info.attachmentCount = 1;
        info.pAttachments = attachment;
        info.width = wd->Width;
        info.height = wd->Height;
        info.layers = 1;
        info.pNext = nullptr;
        info.flags = 0;
        MAX_FRAMES_IN_FLIGHTS_LOOP(i)
        {
            attachment[0] = wd->Frames[i].BackbufferView;
            auto err = vkCreateFramebuffer(GDevice, &info,
                GAllocatorCallback, &PostProcessFrameBuffer[i]);
            if(err != VkResult::VK_SUCCESS)
            {
                ERR("Create FrameBuffer Failed", err);
            }
        }
    }
}

void RenderPostProcessing::ReleaseRenderPass()
{
    vkDestroyRenderPass(GDevice, PostProcessRenderPass, GAllocatorCallback);
}

void RenderPostProcessing::Update()
{
    
}

void RenderPostProcessing::CmdRender(VkCommandBuffer cmd, uint32 FrameIndex)
{
    // Translucent
    {
        std::array<VkClearValue, 1> clearValues = {};
        clearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
        // clearValues[1].depthStencil = {1.0f, 0};
        
        VkRenderPassBeginInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        info.renderPass = PostProcessRenderPass;
        info.framebuffer = PostProcessFrameBuffer[FrameIndex];
        // info.framebuffer = fd->Framebuffer;
        info.renderArea.extent.width = GWindowData.Width;
        info.renderArea.extent.height = GWindowData.Height;
        info.clearValueCount = (uint32)clearValues.size();
        info.pClearValues = clearValues.data();
        
        vkCmdBeginRenderPass(cmd, &info, VK_SUBPASS_CONTENTS_INLINE);
    }
    FrameRenderInfo RenderInfo {
        cmd,
        VkHelperInstance->swapChainExtent,
        {}
    };

    
    mat->Draw(RenderInfo);
    vkCmdDraw(cmd, 6, 1, 0, 0);
    
    // Submit command buffer
    vkCmdEndRenderPass(cmd);
}

class PostProssingMaterialRenderPipelineInfo : public MaterialRenderPipelineInfo
{
public:
    RenderPostProcessing* owner;
    PostProssingMaterialRenderPipelineInfo(RenderPostProcessing* owner, const char* pipe)
        :MaterialRenderPipelineInfo(pipe),owner (owner)
    {
        
    }
    
    VkPipelineVertexInputStateCreateInfo*
    PipelineVertexInputStateCreateInfo(MeshVertexBuffer* VertexBuffer) const override
    {
        static VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 0;
        vertexInputInfo.vertexAttributeDescriptionCount = 0;
        vertexInputInfo.pVertexBindingDescriptions = nullptr;
        vertexInputInfo.pVertexAttributeDescriptions = nullptr;

        if(VertexBuffer)
        {
            VertexBuffer->SetBufferStep<MeshVertex>();
        }
    
        return &vertexInputInfo;
    }

    virtual VkRenderPass PipelineRenderPass() const override
    {
        return owner->PostProcessRenderPass;
    }
};


RenderPostProcessing::RenderPostProcessing()
{
    CreateRenderPass();
    auto matPipe =
        NewSPtr<PostProssingMaterialRenderPipelineInfo>(this, "Nothing");
    
    mat = NewSPtr<Material>(matPipe);

    MAX_FRAMES_IN_FLIGHTS_LOOP(i)
    {
        mat->SetTextureAtFream("texture0",
            ExternalImage(
                VkHelperInstance->Rendering->GetSceneColor(i)
            ),
            i,
            false);
    }
}

RenderPostProcessing::~RenderPostProcessing()
{
    ReleaseRenderPass();

    MAX_FRAMES_IN_FLIGHTS_LOOP(i)
    {
        vkDestroyFramebuffer(GDevice, PostProcessFrameBuffer[i], GAllocatorCallback);
    }
    
    // MAX_FRAMES_IN_FLIGHTS_LOOP(i)
    // {
    //     mat->SetTextureAtFream("texture0",
    //         VK_NULL_HANDLE,
    //         i,
    //         false);
    // }
}

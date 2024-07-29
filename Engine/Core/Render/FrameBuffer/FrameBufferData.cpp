#include "FrameBufferData.h"

#include "Engine/vEngine.h"

// FrameBufferDataCreateInfo info;
// info.width = VkHelperInstance->swapChainExtent.width;
// info.Hight = VkHelperInstance->swapChainExtent.height;
// info.Flags = 0;
// info.Format = VkHelperInstance->findColorAttachmentFormat();
// info.Tiling = VK_IMAGE_TILING_OPTIMAL;
// info.TargetRenderPass = OpaqueRenderPass;
// OpaqueRenderPassFramebuffers = FrameBufferData::CreateFrameBuffer(info);

FrameBufferData::FrameBufferData(const FrameBufferDataCreateInfo& CreateInfo)
{
    if(CreateInfo.TargetRenderPass == VK_NULL_HANDLE)
    {
        throw std::runtime_error("RenderPass Is Null When Create FrameBuffer");
        return;
    }
    
    FrameBufferData* ret = this;
    // Image
    bool useOtherImage = CreateInfo.TargetImage.size() != MAX_FRAMES_IN_FLIGHTS;
    if(!useOtherImage)
    {

        //创建image
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.flags = CreateInfo.Flags;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = CreateInfo.width;
        imageInfo.extent.height = CreateInfo.Hight;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = CreateInfo.Format; // VkHelperInstance->findColorAttachmentFormat();
        imageInfo.tiling = CreateInfo.Tiling; 
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;//GPU无法使用，但是第一个transition将保留texels
        imageInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    
        MAX_FRAMES_IN_FLIGHTS_LOOP(i)
        {
            if (vkCreateImage(GDevice, &imageInfo, nullptr, &ret->Image[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create image!");
            }
        }

        //获取image对应的内存要求（找到对应的内存类型）
        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(GDevice, ret->Image[0], &memRequirements);
        //分配内存
        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size * MAX_FRAMES_IN_FLIGHTS;
        allocInfo.memoryTypeIndex = VkHelperInstance->FindMemoryType(memRequirements.memoryTypeBits,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        if (vkAllocateMemory(GDevice, &allocInfo, nullptr, &ret->BufferMemory) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate image memory!");
        }
        
        //绑定image和为其分配的实际内存
        MAX_FRAMES_IN_FLIGHTS_LOOP(i)
        {
            vkBindImageMemory(GDevice, ret->Image[i],
                ret->BufferMemory, i * memRequirements.size);
        }
    }
    
    {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = CreateInfo.Format;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;
        
        MAX_FRAMES_IN_FLIGHTS_LOOP(i)
        {
            viewInfo.image = useOtherImage ? ret->Image[i] : CreateInfo.TargetImage[i];
            if (vkCreateImageView(GDevice, &viewInfo, nullptr, &ret->ImageView[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create texture image view!");
            }
        }
    }

    // FrameBuffer
    {
        VkImageView attachment[1];
        VkFramebufferCreateInfo info;
        info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        info.renderPass = CreateInfo.TargetRenderPass;
        info.attachmentCount = 1;
        info.pAttachments = attachment;
        info.width = CreateInfo.width;
        info.height = CreateInfo.Hight;
        info.layers = 1;
        MAX_FRAMES_IN_FLIGHTS_LOOP(i)
        {
            attachment[0] = ret->ImageView[i];
            auto err = vkCreateFramebuffer(GDevice, &info, GAllocatorCallback, &ret->FrameBuffer[i]);
            if(err != VkResult::VK_SUCCESS)
            {
                ERR("Create FrameBuffer Failed", err);
            }
        }
    }

}

FrameBufferData::~FrameBufferData()
{
    MAX_FRAMES_IN_FLIGHTS_LOOP(i)
    {
        if(FrameBuffer[i] != VK_NULL_HANDLE)
        {
            vkDestroyFramebuffer(GDevice, FrameBuffer[i], GAllocatorCallback);
            FrameBuffer[i] = VK_NULL_HANDLE;
        }
        if(Image[i] != VK_NULL_HANDLE)
        {
            vkDestroyImage(GDevice, Image[i], GAllocatorCallback);
            FrameBuffer[i] = VK_NULL_HANDLE;
        }
        if(ImageView[i] != VK_NULL_HANDLE)
        {
            vkDestroyImageView(GDevice, ImageView[i], GAllocatorCallback);
            FrameBuffer[i] = VK_NULL_HANDLE;
        }
    }
    
    if(BufferMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(GDevice, BufferMemory, GAllocatorCallback);
    }
    
}

#pragma once
#include <array>
#include <vector>

#include "Header.h"
#include <vulkan/vulkan_core.h>

struct FrameBufferDataCreateInfo
{
    uint32 width;
    uint32 Hight;
    VkFormat Format;
    VkFlags Flags = 0;
    VkImageTiling Tiling;

    VkRenderPass TargetRenderPass;
    
    /// Nullable
    std::vector<VkImage> TargetImage;
    /// Nullable
};

struct FrameBufferData
{
    VkFramebuffer FrameBuffer[MAX_FRAMES_IN_FLIGHTS] = {VK_NULL_HANDLE};
    VkImage Image[MAX_FRAMES_IN_FLIGHTS] = {VK_NULL_HANDLE};
    VkImageView ImageView[MAX_FRAMES_IN_FLIGHTS] = {VK_NULL_HANDLE};
    VkDeviceMemory BufferMemory = {VK_NULL_HANDLE};
    VkRenderPass TargetRenderPass = {VK_NULL_HANDLE};

    VkFramebuffer operator[](uint32 FrameIndex)
    {
        return FrameBuffer[FrameIndex];
    }
    
    static SPtr<FrameBufferData> CreateFrameBuffer(const FrameBufferDataCreateInfo& CreateInfo)
    {
        return NewSPtr<FrameBufferData>(CreateInfo);
    }
    
    ~FrameBufferData();

    FrameBufferData(const FrameBufferDataCreateInfo& CreateInfo);
};

#pragma once
#include <vulkan/vulkan_core.h>

class MemoryBuffer
{
    VkBuffer Buffer;

    VkDeviceMemory vertexBufferMemory;
    
public:
    MemoryBuffer(size_t Size, const void* Data);
    ~MemoryBuffer();
    
    void CmdBind(VkCommandBuffer CommandBuffer);

};

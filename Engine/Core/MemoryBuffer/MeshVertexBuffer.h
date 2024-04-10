#pragma once
#include <vulkan/vulkan_core.h>

class MeshVertexBuffer
{
    VkBuffer Buffer;

    VkDeviceMemory vertexBufferMemory;
    
public:
    MeshVertexBuffer(size_t Size, const void* Data);
    ~MeshVertexBuffer();
    
    void CmdBind(VkCommandBuffer CommandBuffer);

};

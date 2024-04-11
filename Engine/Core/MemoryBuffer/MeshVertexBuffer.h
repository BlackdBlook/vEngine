#pragma once
#include <vulkan/vulkan_core.h>
#include "Header.h"
struct MeshVertex
{
    glm::vec3 Pos;
    glm::vec3 Normal;
    glm::vec2 TexCoords;

    static VkVertexInputBindingDescription getBindingDescription();

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();
};

class MeshVertexBuffer
{
    VkBuffer Buffer;

    VkDeviceMemory vertexBufferMemory;
    
public:
    MeshVertexBuffer(size_t Size, const void* Data);
    ~MeshVertexBuffer();
    
    void CmdBind(VkCommandBuffer CommandBuffer);

};

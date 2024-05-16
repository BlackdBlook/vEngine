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

    static size_t GetVertexStep();
};

class MeshVertexBuffer
{
    VkBuffer Buffer;

    VkDeviceMemory vertexBufferMemory;

    size_t Size = 0;

    size_t BufferStep = 8;
    
public:
    MeshVertexBuffer(size_t Size, const void* Data);
    ~MeshVertexBuffer();
    
    void CmdBind(VkCommandBuffer CommandBuffer);

    template<typename T = MeshVertex>
    void SetBufferStep()
    {
        BufferStep = T::GetVertexStep();
    }

    uint32 GetVertexNumber()
    {
        return static_cast<uint32>(Size / BufferStep);
    }
};

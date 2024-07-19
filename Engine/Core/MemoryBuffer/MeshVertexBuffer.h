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

class VertexBuffer
{
protected:
    VkBuffer Buffer = VK_NULL_HANDLE;

    VkDeviceMemory vertexBufferMemory = VK_NULL_HANDLE;

    size_t Size = 0;

    size_t BufferStep = 8;

    virtual ~VertexBuffer();

    
public:
    virtual VkBuffer GetVkBuffer()
    {
        return Buffer;
    }

    uint32 GetVertexNumber()
    {
        return static_cast<uint32>(Size / BufferStep);
    }

    virtual void CmdBind(VkCommandBuffer CommandBuffer) = 0;

    template<typename T = MeshVertex>
    void SetBufferStep()
    {
        BufferStep = T::GetVertexStep();
    }
};

class MeshVertexBuffer : public VertexBuffer
{
public:
    MeshVertexBuffer(size_t Size, const void* Data);
    ~MeshVertexBuffer() override;
    
    virtual void CmdBind(VkCommandBuffer CommandBuffer) override;
};

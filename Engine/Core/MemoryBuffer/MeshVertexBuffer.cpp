#include "MeshVertexBuffer.h"

#include <array>
#include <assert.h>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

#include "Engine/vEngine.h"
#include "LogPrinter/Log.h"


VkVertexInputBindingDescription MeshVertex::getBindingDescription()
{
    VkVertexInputBindingDescription bindingDescription{};

    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(MeshVertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
}

std::array<VkVertexInputAttributeDescription, 3> MeshVertex::getAttributeDescriptions()
{
    std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(MeshVertex, Pos);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(MeshVertex, Normal);

    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[2].offset = offsetof(MeshVertex, TexCoords);
    return attributeDescriptions;
}

size_t MeshVertex::GetVertexStep()
{
    return 3 + 3 + 2;
}

MeshVertexBuffer::MeshVertexBuffer(size_t Size, const void* Data)
{
    if(Size == 0)
    {
        return;
    }

    this->Size = Size;

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = Size;
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(GDevice, &bufferInfo, nullptr, &Buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create vertex buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(GDevice, Buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = VkHelperInstance->FindMemoryType(memRequirements.memoryTypeBits,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if (vkAllocateMemory(GDevice, &allocInfo, nullptr, &vertexBufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate vertex buffer memory!");
    }

    vkBindBufferMemory(GDevice, Buffer, vertexBufferMemory, 0);
    
    void* data;
    vkMapMemory(GDevice, vertexBufferMemory, 0, bufferInfo.size, 0, &data);
    memcpy(data, Data, Size);
    vkUnmapMemory(GDevice, vertexBufferMemory);
}

MeshVertexBuffer::~MeshVertexBuffer()
{
    vkDestroyBuffer(GDevice, Buffer, nullptr);
    vkFreeMemory(GDevice, vertexBufferMemory, nullptr);
}

void MeshVertexBuffer::CmdBind(VkCommandBuffer CommandBuffer)
{
    assert(CommandBuffer != nullptr);
    VkBuffer vertexBuffers[] = {Buffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(CommandBuffer, 0, 1, vertexBuffers, offsets);
}


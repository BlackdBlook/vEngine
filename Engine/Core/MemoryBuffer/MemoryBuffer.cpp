#include "MemoryBuffer.h"

#include <assert.h>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

#include "Engine/vEngine.h"

MemoryBuffer::MemoryBuffer(size_t Size, const void* Data)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = Size;
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(GlobalVkLogicDevice, &bufferInfo, nullptr, &Buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create vertex buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(GlobalVkLogicDevice, Buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = VkHelperInstance->FindMemoryType(memRequirements.memoryTypeBits,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if (vkAllocateMemory(GlobalVkLogicDevice, &allocInfo, nullptr, &vertexBufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate vertex buffer memory!");
    }

    vkBindBufferMemory(GlobalVkLogicDevice, Buffer, vertexBufferMemory, 0);

    void* data;
    vkMapMemory(GlobalVkLogicDevice, vertexBufferMemory, 0, bufferInfo.size, 0, &data);
    memcpy(data, Data, Size);
    vkUnmapMemory(GlobalVkLogicDevice, vertexBufferMemory);
}

MemoryBuffer::~MemoryBuffer()
{
    vkDestroyBuffer(GlobalVkLogicDevice, Buffer, nullptr);
    vkFreeMemory(GlobalVkLogicDevice, vertexBufferMemory, nullptr);
}

void MemoryBuffer::CmdBind(VkCommandBuffer CommandBuffer)
{
    assert(CommandBuffer != nullptr);
    VkBuffer vertexBuffers[] = {Buffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(CommandBuffer, 0, 1, vertexBuffers, offsets);
}


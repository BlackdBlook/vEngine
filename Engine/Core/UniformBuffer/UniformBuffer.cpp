#include "UniformBuffer.h"

#include <stdexcept>

#include "Header.h"
#include "Engine/vEngine.h"

UniformBuffer::UniformBuffer()
{
    
}

UniformBuffer::~UniformBuffer()
{
    //销毁Uniform buffer和对应Memory
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHTS; i++)
    {
        vkDestroyBuffer(GDevice, uniformBuffers[i], nullptr);
        vkFreeMemory(GDevice, uniformBuffersMemory[i], nullptr);
    }
    
}

void UniformBuffer::Init(size_t size, string Name, uint32 bind)
{
    BlockName = Name;
    Bind = bind;
    BufferSize = size;
    uniformBuffers.resize(MAX_FRAMES_IN_FLIGHTS);
    uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHTS);
    uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHTS);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHTS; i++)
    {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(GDevice, &bufferInfo, nullptr, &uniformBuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create vertex buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(GDevice, uniformBuffers[i], &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = VkHelperInstance->FindMemoryType(memRequirements.memoryTypeBits,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        if (vkAllocateMemory(GDevice, &allocInfo, nullptr, &uniformBuffersMemory[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate vertex buffer memory!");
        }

        vkBindBufferMemory(GDevice, uniformBuffers[i], uniformBuffersMemory[i], 0);

        // 调用Map会消耗额外资源，所以可以直接Map而不UnMap
        vkMapMemory(GDevice, uniformBuffersMemory[i], 0, bufferInfo.size, 0, &pData[i]);
    }
}

void UniformBuffer::UpdateCurrentBuffer(void* data, size_t size, size_t offset)
{
    assert(pData);

    assert(this->BufferSize >= offset + size);

    uint8* dist = static_cast<uint8*>(pData[Engine::ins->GetCurrentFrame()]);
    
    memcpy(dist + offset, data, size);
}

void UniformBuffer::UpdateAllBuffer(void* data, size_t size, size_t offset)
{
    assert(pData);

    assert(this->BufferSize >= offset + size);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHTS; i++)
    {
        uint8* dist = static_cast<uint8*>(pData[i]);
        memcpy(dist + offset, data, size);
    }
}

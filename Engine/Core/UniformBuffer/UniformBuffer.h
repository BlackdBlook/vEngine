#pragma once
#include <vector>

#include "Header.h"
#include "Engine/TypeDef.h"
#include "Engine/Core/MemoryBuffer/MeshVertexBuffer.h"

class UniformBuffer
{
    
public:
    UniformBuffer();
    ~UniformBuffer();
    void Init(size_t size);

    void UpdateBuffer(void* data, size_t size, uint32 offset = 0);

    VkDeviceSize GetBufferSize() const {return BufferSize;}

    const VkBuffer* GetUniformBuffer(size_t index) const {return &uniformBuffers[index];}

private:
    VkDeviceSize BufferSize = 0;

    //uniform buffers，数量和同时运行的frame数量一致
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;

    void* pData[MAX_FRAMES_IN_FLIGHTS];
};

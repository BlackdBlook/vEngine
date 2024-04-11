#pragma once
#include <unordered_map>
#include <vector>

#include "Header.h"
#include "Engine/TypeDef.h"
#include "Engine/Core/MemoryBuffer/MeshVertexBuffer.h"

class UniformBuffer
{
    
public:
    UniformBuffer();
    UniformBuffer(const UniformBuffer& other) = delete;
    UniformBuffer(UniformBuffer&& other) = default;
    ~UniformBuffer();
    void Init(size_t size, string Name, uint32 bind);

    void UpdateBuffer(void* data, size_t size, uint32 offset = 0);

    VkDeviceSize GetBufferSize() const {return BufferSize;}

    const VkBuffer* GetUniformBuffer(size_t index) const {return &uniformBuffers[index];}

    string BlockName;

    uint32 Bind = 0;

private:
    VkDeviceSize BufferSize = 0;

    //uniform buffers，数量和同时运行的frame数量一致
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;

    void* pData[MAX_FRAMES_IN_FLIGHTS];
};

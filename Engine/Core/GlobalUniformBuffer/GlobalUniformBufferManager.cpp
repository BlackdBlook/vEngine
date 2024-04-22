#include "GlobalUniformBufferManager.h"

#include "Engine/vEngine.h"

void GlobalUniformBuffer::Init(size_t size, string Name, uint32 bind)
{
    assert(false);
}

void GlobalUniformBuffer::Init_Internal(size_t size, string Name)
{
    BlockName = "GlobalUniformBuffer";
    UniformBuffer::Init(size, Name, 0);
}

GlobalUniformBufferManager::GlobalUniformBufferManager()
{
    
}

SPtr<GlobalUniformBuffer> GlobalUniformBufferManager::GetBuffer(size_t size, ShaderUniformBufferBlock* block)
{
    if(buffer == nullptr)
    {
        Init(size, "GlobalUniformBuffer");
        buffer->UniformBlockCache = *block;
    }

    assert(buffer->GetBufferSize() == size);
    assert((buffer->UniformBlockCache) == *block);
    
    return buffer;
}

GlobalUniformBufferManager* GlobalUniformBufferManager::Get()
{
    static GlobalUniformBufferManager* manager = new GlobalUniformBufferManager();

    return manager;
}

void GlobalUniformBufferManager::cleanUp()
{
    buffer.reset();
}

void GlobalUniformBufferManager::Init(size_t size, string Name)
{
    buffer = std::make_shared<GlobalUniformBuffer>();
    buffer->Init_Internal(size, Name);
}

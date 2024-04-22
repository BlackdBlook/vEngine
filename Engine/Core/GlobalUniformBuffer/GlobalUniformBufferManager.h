#pragma once
#include <vector>

#include "Engine/Core/ShaderModule/Shader.h"
#include "Engine/Core/UniformBuffer/UniformBuffer.h"
#include "LogPrinter/Log.h"


class GlobalUniformBuffer : public UniformBuffer
{
    friend class GlobalUniformBufferManager;
    virtual void Init(size_t size, string Name, uint32 bind) override;

    void Init_Internal(size_t size, string Name);
};
 
class GlobalUniformBufferManager
{
public:
    GlobalUniformBufferManager();

    SPtr<GlobalUniformBuffer> GetBuffer(size_t size, ShaderUniformBufferBlock* block);

    std::unordered_map<string, size_t> MemberOffset;
    
    static GlobalUniformBufferManager* Get();

    void UpdateCurrentBuffer(const void* data, size_t size, size_t offset = 0)
    {
        buffer->UpdateCurrentBuffer(data, size, offset);
    }
    
    void UpdateAllBuffer(const void* data, size_t size, size_t offset = 0)
    {
        buffer->UpdateCurrentBuffer(data, size, offset);
    }

    template<typename T>
    void SetCurrentBuffer(const string& name, const T& data);

    template<typename T>
    void SetAllBuffer(const string& name, const T& data);

    void cleanUp();

private:
    
    SPtr<GlobalUniformBuffer> buffer;
    
    void Init(size_t size, string Name);
};

template <typename T>
void GlobalUniformBufferManager::SetCurrentBuffer(const string& name, const T& data)
{
    auto member = buffer->UniformBlockCache.Members.find(name);

    if(member == buffer->UniformBlockCache.Members.end())
    {
        WARNING("Global Uniform member not found", name);
        return;
    }
    
    UpdateCurrentBuffer(&data, sizeof(T), member->second.Offset);
}

template <typename T>
void GlobalUniformBufferManager::SetAllBuffer(const string& name, const T& data)
{
    auto member = buffer->UniformBlockCache.Members.find(name);

    if(member == buffer->UniformBlockCache.Members.end())
    {
        WARNING("Global Uniform member not found", name);
        return;
    }
    
    UpdateAllBuffer(&data, sizeof(T), member->second.Offset);
}

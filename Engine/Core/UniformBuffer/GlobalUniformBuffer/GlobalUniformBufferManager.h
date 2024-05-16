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

    void UpdateCameraData();
public:
    
    UniformMemberMap Members;
};


 
class GlobalUniformBufferManager
{
    friend GlobalUniformBuffer;
public:
    GlobalUniformBufferManager();
    ~GlobalUniformBufferManager();

    SPtr<GlobalUniformBuffer> GetBuffer(size_t size, ShaderUniformBufferBlock* block);

    GlobalUniformBufferInfo* globalInfo;
    
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

    void UpdateCameraData()
    {
        if(buffer)
        {
            buffer->UpdateCameraData();
        }
    }


private:
    
    SPtr<GlobalUniformBuffer> buffer;
    
    void Init(size_t size, string Name, ShaderUniformBufferBlock* block);
};

template <typename T>
void GlobalUniformBufferManager::SetCurrentBuffer(const string& name, const T& data)
{
    auto member = buffer->Members.find(name);

    if(member == buffer->Members.end())
    {
        WARNING("Global Uniform member not found", name);
        return;
    }
    
    UpdateCurrentBuffer(&data, sizeof(T), member->second.Offset);
}

template <typename T>
void GlobalUniformBufferManager::SetAllBuffer(const string& name, const T& data)
{
    auto member = buffer->Members.find(name);

    if(member == buffer->Members.end())
    {
        WARNING("Global Uniform member not found", name);
        return;
    }
    
    UpdateAllBuffer(&data, sizeof(T), member->second.Offset);
}

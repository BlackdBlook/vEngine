#include "GlobalUniformBufferManager.h"

#include "Engine/vEngine.h"
#include "Engine/Core/Camera/Camera.h"

void GlobalUniformBuffer::Init(size_t size, string Name, uint32 bind)
{
    assert(false);
}

void GlobalUniformBuffer::Init_Internal(size_t size, string Name)
{
    BlockName = "GlobalUniformBuffer";
    UniformBuffer::Init(size, Name, 0);

}

void GlobalUniformBuffer::UpdateCameraData()
{
    Camera* cam = Camera::GetCamera();
    
    {
        auto view = cam->GetCameraView();
        auto member = UniformBlockCache.Members.find("u_View");

        if(member == UniformBlockCache.Members.end())
        {
            WARNING("Global Uniform member not found", member->first);
            return;
        }
    
        UpdateAllBuffer(&view, sizeof(view), member->second.Offset);
    }

    {
        auto Projection = cam->GetCameraProjection();
        auto member = UniformBlockCache.Members.find("u_Projection");

        if(member == UniformBlockCache.Members.end())
        {
            WARNING("Global Uniform member not found", member->first);
            return;
        }
    
        UpdateAllBuffer(&Projection, sizeof(Projection), member->second.Offset);
    }
}

GlobalUniformBufferManager::GlobalUniformBufferManager()
{
    
}

SPtr<GlobalUniformBuffer> GlobalUniformBufferManager::GetBuffer(size_t size, ShaderUniformBufferBlock* block)
{
    if(buffer == nullptr)
    {
        Init(size, "GlobalUniformBuffer", block);
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

void GlobalUniformBufferManager::Init(size_t size, string Name, ShaderUniformBufferBlock* block)
{
    buffer = std::make_shared<GlobalUniformBuffer>();
    buffer->Init_Internal(size, Name);
    buffer->UniformBlockCache = *block;
    buffer->UpdateCameraData();
}

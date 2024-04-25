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
        auto member = Members.find("Global.u_View");

        if(member == Members.end())
        {
            WARNING("Global Uniform member not found", "Global.u_View");
            return;
        }
    
        UpdateAllBuffer(&view, sizeof(view), member->second.Offset);
    }

    {
        auto Projection = cam->GetCameraProjection();
        auto member = Members.find("Global.u_Projection");

        if(member == Members.end())
        {
            WARNING("Global Uniform member not found", "Global.u_Projection");
            return;
        }
    
        UpdateAllBuffer(&Projection, sizeof(Projection), member->second.Offset);
    }

    {
        auto pos = cam->GetPos();
        auto member = Members.find("Global.u_CameraPos");

        if(member == Members.end())
        {
            WARNING("Global Uniform member not found", "Global.u_CameraPos");
            return;
        }
    
        UpdateAllBuffer(&pos, sizeof(pos), member->second.Offset);
    }
}

GlobalUniformBufferManager::GlobalUniformBufferManager()
{
    
}

GlobalUniformBufferManager::~GlobalUniformBufferManager()
{
    delete globalInfo;
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
    buffer->Members = globalInfo->Members;
    buffer->UpdateCameraData();
}

#include "RenderCommandQueue.h"

#include <map>

#include "SceneComponentRenderInfo.h"
#include "Engine/Core/Camera/Camera.h"
#include "Engine/Core/Component/SceneComponent/SceneComponent.h"
#include "Engine/Core/Material/Material.h"

void RenderCommandQueue::PushRenderCommand(FrameInfo& NewCommand)
{
    vec3 CameraPos = Camera::GetCamera()->GetPos();
    
    while(NewCommand.SceneComponentRenderInfos.IsEmpty() == false)
    {
        SceneComponentRenderInfo* info;
        NewCommand.SceneComponentRenderInfos.Dequeue(info);

        vec3 worldPos = info->component->GetWorldPos();
        
        float camDistance = glm::distance(CameraPos, worldPos);

        switch (info->material->info->RenderType)
        {
        case MaterialRenderType::Opaque:
            OpaqueOrderInfo.insert({camDistance, info});
            break;
        case MaterialRenderType::Translucent:
            TranslucentOrderInfo.insert({camDistance, info});
            break;
        case MaterialRenderType::Sky:
            SkyRenderInfo = info;
            break;
        }
    }

}


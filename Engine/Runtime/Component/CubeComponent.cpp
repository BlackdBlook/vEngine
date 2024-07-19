#include "CubeComponent.h"
#include "Engine/Core/FrameInfo/FrameRenderInfo.h"
#include "Engine/Core/Object/Object.h"
#include "Engine/Core/Render/SceneComponentRenderInfo.h"
#include "Meshs/Box/BoxVertices.h"

CubeComponent::CubeComponent(const string& shader):
buffer(sizeof(BoxVertices), BoxVertices)
{
    material = NewSPtr<Material>(shader);
}

CubeComponent::~CubeComponent()
{
}

void CubeComponent::Update(float DeltaTime)
{
    SceneComponent::Update(DeltaTime);
    
}

void CubeComponent::Draw(FrameRenderInfo& RenderInfo)
{
    SceneComponent::Draw(RenderInfo);

    auto model = ParentObject->GetModelMat();

    material->SetCurrentUniformData("model.model", model);
    
    // material->Draw(RenderInfo);
    //
    // buffer.CmdBind(RenderInfo.CommmandBuffer);
    //
    // vkCmdDraw(RenderInfo.CommmandBuffer,
    //     buffer.GetVertexNumber(), 1, 0, 0);

    RenderInfo.SceneComponentRenderInfos.Enqueue(GenRenderInfo());
}

SceneComponentRenderInfo* CubeComponent::GenRenderInfo()
{
    if(renderInfoCache == nullptr)
    {
        renderInfoCache = new SceneComponentRenderInfo();
        renderInfoCache->component = this;
        renderInfoCache->material = material.get();
        renderInfoCache->Model = GetModelMat();
        renderInfoCache->VertexBuffer = &buffer;
    }

    return renderInfoCache;
}

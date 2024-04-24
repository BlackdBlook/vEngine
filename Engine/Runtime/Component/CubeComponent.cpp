#include "CubeComponent.h"
#include "Engine/Core/FrameInfo/RenderInfo.h"
#include "Engine/Core/Object/Object.h"
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
    Component::Update(DeltaTime);
    
}

void CubeComponent::Draw(const RenderInfo& RenderInfo)
{
    Component::Draw(RenderInfo);

    auto model = Parent->GetModelMat();

    material->SetCurrentUniformData("model", "model", model);
    
    material->Draw(RenderInfo);

    buffer.CmdBind(RenderInfo.CommmandBuffer);
    
    vkCmdDraw(RenderInfo.CommmandBuffer,
        sizeof(BoxVertices) / 8, 1, 0, 0);
    
}

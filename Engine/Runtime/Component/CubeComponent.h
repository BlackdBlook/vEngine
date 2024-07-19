#pragma once
#include "Engine/Core/Component/Component.h"
#include "Engine/Core/Component/SceneComponent/SceneComponent.h"
#include "Engine/Core/Material/Material.h"


class CubeComponent : public SceneComponent
{
public:
    SPtr<Material> material;

    MeshVertexBuffer buffer;
public:
    CubeComponent(const string& shader);

    ~CubeComponent() override;

    virtual void Update(float DeltaTime) override;

    virtual void Draw(FrameRenderInfo& RenderInfo) override;

    virtual SceneComponentRenderInfo* GenRenderInfo() override;
};

#pragma once
#include "Engine/Core/Component/Component.h"
#include "Engine/Core/Material/Material.h"


class CubeComponent : public Component
{
public:
    SPtr<Material> material;

    MeshVertexBuffer buffer;
public:
    CubeComponent(const string& shader);

    ~CubeComponent() override;

    virtual void Update(float DeltaTime) override;

    virtual void Draw(const RenderInfo& RenderInfo) override; 
};

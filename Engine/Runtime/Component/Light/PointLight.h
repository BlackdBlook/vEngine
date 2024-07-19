#pragma once
#include "Engine/Core/Component/Component.h"

class PointLight : public Component
{
public:
    virtual void Update(float DeltaTime) override;
    virtual void Draw(FrameRenderInfo& RenderInfo) override;
};

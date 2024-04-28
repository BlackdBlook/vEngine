#pragma once
#include "Engine/Core/Component/Component.h"

class PressEscExit : public Component
{
public:
    virtual void Update(float DeltaTime) override;
};

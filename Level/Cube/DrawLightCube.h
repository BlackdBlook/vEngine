#pragma once
#include "Engine/Core/Level/Level.h"

class Material;

class DrawLightCube : public Level
{
    Material* m;
public:
    virtual void Init() override;
};

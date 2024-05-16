#pragma once
#include "Engine/TypeDef.h"
#include "Engine/Toolkit/math_utils.h"

class Material;
class SceneComponent;

class SceneComponentRenderInfo
{
public:
    SceneComponent* component;
    Material* material;

    MAT4(Model);
};

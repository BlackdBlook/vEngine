#pragma once
#include "Engine/TypeDef.h"
#include "Engine/Core/UniformBuffer/GlobalUniformBuffer/GlobalUniformBufferManager.h"
#include "Engine/Toolkit/math_utils.h"

class Material;
class SceneComponent;

class SceneComponentRenderInfo
{
public:
    SceneComponent* component;
    Material* material;
    VertexBuffer* VertexBufferPtr;
    MAT4(Model);
};

#pragma once
#include "Engine/TypeDef.h"
#include "Engine/Core/UniformBuffer/GlobalUniformBuffer/GlobalUniformBufferManager.h"
#include "Engine/Toolkit/math_utils.h"

class MeshVertexBuffer;
class Material;
class SceneComponent;

class SceneComponentRenderInfo
{
public:
    SceneComponent* component;
    Material* material;
    MeshVertexBuffer* VertexBuffer;
    MAT4(Model);
};

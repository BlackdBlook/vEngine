#pragma once
#include <vulkan/vulkan_core.h>

#include "Engine/TypeDef.h"

class RenderInfo;
class Object;

class Component
{
    friend Object;
    WPtr<Object> Parent;
public:
    virtual void OnAttach(){}
    virtual void OnDetach(){}
    virtual void Update(float DeltaTime);
    virtual void LateUpdate(float DeltaTime);
    virtual void Draw(const RenderInfo& RenderInfo);
    virtual ~Component();
};

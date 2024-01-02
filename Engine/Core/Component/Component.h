#pragma once
#include "Engine/TypeDef.h"

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
    virtual void Draw();
    virtual ~Component();
};

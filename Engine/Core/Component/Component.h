#pragma once
#include <memory>
#include "Header.h"

class RenderInfo;
class Object;

class Component
{
public:
    Object* Parent = nullptr;
    
    bool operator==(Component* c);
    bool operator==(const char* c);
    virtual void OnAttached();
    virtual void OnDettached();
    virtual void Draw(const RenderInfo& RenderInfo) {}
    virtual void Update(float DeltaTime) {}
    virtual void Destory() {}
    virtual ~Component() = default; 
};


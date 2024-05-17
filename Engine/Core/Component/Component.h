#pragma once
#include <memory>
#include "Header.h"

class FrameInfo;
class Object;

class Component : public std::enable_shared_from_this<Component>
{
public:
    Object* ParentObject = nullptr;
    
    bool operator==(Component* c);
    bool operator==(const char* c);
    virtual void OnAttached();
    virtual void OnDettached();
    virtual void Draw(FrameInfo& RenderInfo) {}
    virtual void Update(float DeltaTime) {}
    virtual void Destory() {}
    virtual ~Component() = default; 
};


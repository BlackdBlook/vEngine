#pragma once
#include <memory>
#include "Header.h"

class RenderInfo;
class Object;

class Component
{
public:
    
    std::weak_ptr<Object> Parent;
    
    bool operator==(Component* c);
    bool operator==(const char* c);
    virtual void OnAttached();
    virtual void OnDettached();
    virtual glm::vec3 GetPos();
    virtual glm::quat GetRot();
    virtual glm::vec3 GetScale();
    virtual void Draw(const RenderInfo& RenderInfo) {}
    virtual void Update(float DeltaTime) {}
    virtual void Destory() {}
    virtual ~Component() = default; 
};


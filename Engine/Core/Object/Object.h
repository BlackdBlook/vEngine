#pragma once
#include <vector>

#include "Engine/TypeDef.h"

class Object
{
    std::vector<SPtr<class Component>> Components;
    
public:

    virtual void AttachComponent(const SPtr<Component>& Target);
    virtual void DetachComponent(const SPtr<Component>& Target);

    template<typename T>
    void GetComponents(std::vector<SPtr<T>>& out);
    
    virtual void Update(float DeltaTime);
    virtual void LateUpdate(float DeltaTime);
    virtual void Draw();
    virtual ~Object();
};

template <typename T>
void Object::GetComponents(std::vector<SPtr<T>>& out)
{
    static_assert(std::is_base_of_v<Component, T>, "GetComponents Must Return Component");

    std::vector<SPtr<T>> ans;

    for(auto& c : Components)
    {
        auto p = std::dynamic_pointer_cast<T>(c);
        if(p != nullptr)
        {
            out.emplace_back(std::move(p));
        }
    }
    
    out = std::move(ans);
}

#pragma once
#include <vector>
#include <glm/detail/type_quat.hpp>

#include "Engine/Core/Component/Component.h"
#include "Engine/Core/Container/Transform.h"

class SceneComponentRenderInfo;
struct Transform;

#define Set(Type, Name, Var) void Set##Type##Name(const Var& new##Name)
#define Get(Ret, Type, Name) Ret Get##Type##Name()const

class SceneComponent : public Component
{
    friend Object;
    SceneComponent* ParentComponent = nullptr;
    Transform relativeTransform;

    SceneComponentRenderInfo* renderInfoCache = nullptr;
    
public:
    std::vector<SPtr<SceneComponent>> ChildComponents;

    SceneComponent();
    ~SceneComponent() override;
    
    Get(vec3, Relative ,Pos)
    {
        return relativeTransform.pos;
    }
    Get(vec3, World ,Pos)
    {
        return GetWorldTransform().pos;
    }
    Set(Relative, Pos ,vec3)
    {
        relativeTransform.pos = newPos;
    }
    Set(World, Pos ,vec3)
    {
        
    }

    Get(quat, Relative ,Rot)
    {
        return relativeTransform.rot;
    }
    Get(quat, World ,Rot)
    {
        return GetWorldTransform().rot;
    }
    Set(Relative, Rot ,vec3)
    {
        relativeTransform.rot = newRot;
    }
    Set(World, Rot ,vec3)
    {
        
    }
    Set(Relative, Rot ,quat)
    {
        relativeTransform.rot = newRot;
    }
    Set(World, Rot ,quat)
    {
        
    }
    
    vec3 GetRelativeScale()
    {
        return relativeTransform.scale;
    }
    vec3 GetWorldScale()
    {
        return GetWorldTransform().scale;
    }
    Set(Relative, Scale ,vec3)
    {
        relativeTransform.pos = newScale;
    }
    Set(World, Scale ,vec3)
    {
        
    }

    Transform GetRelativeTransform()
    {
        return relativeTransform;
    }

    Transform GetWorldTransform() const
    {
        return ParentComponent ?
            ParentComponent->GetRelativeTransform() * relativeTransform :
            relativeTransform;
    }

    template<typename T>
    SPtr<T> FindComponent(uint32 StartIndex = 0);
    template <typename T, bool EnbaleOutIndex = false>
    SPtr<T> FindComponentIndex(uint32& OutIndex, uint32 StartIndex = 0);
    template <class T>
    void FindComponents(std::vector<std::shared_ptr<T>>& out);

    uint32 FindComponentIndex(SceneComponent* component) const;

    virtual void Update(float DeltaTime) override;
    virtual void Draw(const RenderInfo& RenderInfo) override;

    virtual void AttachComponent(SPtr<SceneComponent> NewChild);
    virtual void RemoveComponent(SceneComponent* Child);

    // Render
    virtual SceneComponentRenderInfo* GenRenderInfo()
    {
        return nullptr;
    }
    
    void MarkRenderInfoDirty();

    virtual void OnAttached() override;
    virtual void OnDettached() override;
};

template <typename T>
SPtr<T> SceneComponent::FindComponent(uint32 StartIndex)
{
    static_assert(std::is_base_of<SceneComponent, T>::value, "只能查找SceneComponent");
    uint32 i;
    return FindComponentIndex<T,false>(i, StartIndex);
}

template <typename T, bool EnbaleOutIndex>
SPtr<T> SceneComponent::FindComponentIndex(uint32& OutIndex, uint32 StartIndex)
{
    static_assert(std::is_base_of<SceneComponent, T>::value, "只能查找SceneComponent");
    uint32 childNum = ChildComponents.size();
    if constexpr(EnbaleOutIndex)
    {
        OutIndex = UINT_MAX;
    }
    if(StartIndex >= childNum)
    {
        return nullptr;
    }

    for(uint32 i = StartIndex;i < childNum; ++i)
    {
        SPtr<T> ret = std::dynamic_pointer_cast<T>(ChildComponents[i]);
        if(ret != nullptr)
        {
            if constexpr(EnbaleOutIndex)
            {
                OutIndex = i;
            }
            return ret;
        }
    }
    return nullptr;
}

template <typename T>
void SceneComponent::FindComponents(std::vector<SPtr<T>>& out)
{
    static_assert(std::is_base_of<SceneComponent, T>::value, "只能查找SceneComponent");
    uint32 childNum = ChildComponents.size();
    
    for(uint32 i = 0;i < childNum; ++i)
    {
        SPtr<T> ret = std::dynamic_pointer_cast<T>(ChildComponents[i]);
        if(ret != nullptr)
        {
            out.emplace_back(std::move(ret));
        }
    }
}

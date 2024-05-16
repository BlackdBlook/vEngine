#include "SceneComponent.h"

#include "Engine/Core/Render/SceneComponentRenderInfo.h"
#include "LogPrinter/Log.h"

SceneComponent::SceneComponent()
{
    
}

SceneComponent::~SceneComponent()
{
}

uint32 SceneComponent::FindComponentIndex(
    SceneComponent* component)const
{
    if(component->ParentComponent != this)
    {
        return UINT_MAX;
    }
    
    uint32 num = (uint32)ChildComponents.size();
    for(uint32 i = 0;i < num; ++i)
    {
        if(ChildComponents[i].get() == component)
        {
            return i;
        }
    }
    return UINT_MAX;
}

void SceneComponent::Update(float DeltaTime)
{
    for(auto& c : ChildComponents)
    {
        c->Update(DeltaTime);
    }
}

void SceneComponent::Draw(const RenderInfo& RenderInfo)
{
    for(auto& c : ChildComponents)
    {
        c->Draw(RenderInfo);
    }
}

void SceneComponent::AttachComponent(SPtr<SceneComponent> NewChild)
{
    if(ParentComponent == nullptr && ParentObject)
    {
        WARNING("组件还没有附加到Object");
    }

    NewChild->ParentComponent = this;
    NewChild->ParentObject = ParentObject;
    NewChild->OnAttached();
}

void SceneComponent::RemoveComponent(SceneComponent* Child)
{
    if(Child->ParentComponent != this)
    {
        WARNING("目标组件不是当前组件的子组件");
    }
    uint32 index = FindComponentIndex(Child);
    if(index == UINT_MAX)
    {
        ERR("Child 没有被正确添加");
        return;
    }
    ChildComponents.erase(ChildComponents.begin() + index);

    Child->OnDettached();
    Child->ParentComponent = nullptr;
}

void SceneComponent::MarkRenderInfoDirty()
{
    if(renderInfoCache)
    {
        delete renderInfoCache;
        renderInfoCache = nullptr;
    }

    for(auto& c : ChildComponents)
    {
        c->MarkRenderInfoDirty();
    }
}

void SceneComponent::OnAttached()
{
    Component::OnAttached();
    for(auto& c : ChildComponents)
    {
        c->ParentObject = ParentObject;
    }
}

void SceneComponent::OnDettached()
{
    Component::OnDettached();
    for(auto& c : ChildComponents)
    {
        c->ParentObject = ParentObject;
    }
}

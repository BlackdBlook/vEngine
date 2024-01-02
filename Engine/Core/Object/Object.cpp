#include "Object.h"

#include "Engine/Core/Component/Component.h"
#include "Engine/Core/Level/Level.h"

void Object::AttachComponent(const SPtr<Component>& Target)
{
    Components.push_back(Target);
    Target->Parent = Level::CurrentLevel->GetObjectSPtr(this);
    Target->OnAttach();
}

void Object::DetachComponent(const SPtr<Component>& Target)
{
    Target->OnDetach();
    for(auto it = Components.begin(); it != Components.end();)
    {
        if(*it == Target)
        {
            Components.erase(std::move(it));
            return;
        }
    }
}

void Object::Update(float DeltaTime)
{
    for(auto& c : Components)
    {
        c->Update(DeltaTime);
    }
}

void Object::LateUpdate(float DeltaTime)
{
    for(auto& c : Components)
    {
        c->LateUpdate(DeltaTime);
    }
}

void Object::Draw()
{
    for(auto& c : Components)
    {
        c->Draw();
    }
}

Object::~Object()
{
}

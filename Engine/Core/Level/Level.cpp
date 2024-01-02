#include "Level.h"

#include "Engine/Core/Object/Object.h"

Level* Level::CurrentLevel = nullptr;

void Level::Init()
{
}

void Level::Start()
{
    CurrentLevel = this;
}

void Level::Update(float DeltaTime)
{
    for(auto o : ObjectMap)
    {
        o.second->Update(DeltaTime);
    }
}

void Level::LateUpdate(float DeltaTime)
{
    for(auto o : ObjectMap)
    {
        o.second->LateUpdate(DeltaTime);
    }
}

void Level::Draw()
{
    for(auto o : ObjectMap)
    {
        o.second->Draw();
    }
}

Level::~Level()
{
    if(CurrentLevel == this)
    {
        CurrentLevel = nullptr;
    }
}

SPtr<Object> Level::NewObject()
{
    SPtr<Object> ans = NewSPtr<Object>();
    ObjectMap.insert({ans.get(), ans});
    return ans;
}

SPtr<Object> Level::GetObjectSPtr(Object* object)
{
    auto ans = ObjectMap.find(object);
    
    return ans == ObjectMap.end() ? nullptr : ans->second;
}

#include "Level.h"

#include "Engine/Core/FrameInfo/FrameInfo.h"
#include "Engine/Core/Object/Object.h"

Level* Level::CurrentLevel = nullptr;
LevelRegisterList* Level::GlobalLevelRegisterList = nullptr;

void Level::Init()
{
}

void Level::Start()
{

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

void Level::Draw(FrameInfo& RenderInfo)
{
    for(auto o : ObjectMap)
    {
        o.second->Draw(RenderInfo);
    }
}

Level::Level()
{
    CurrentLevel = this;
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

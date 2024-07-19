#pragma once
#include <functional>
#include <unordered_map>
#include <vector>
#include <vulkan/vulkan_core.h>

#include "Engine/TypeDef.h"

class FrameRenderInfo;
class Object;
struct ObjectHandle;

using LevelRegisterList = std::vector<std::function<void(std::shared_ptr<class Level>& CurrentLevel)>>;
class Level
{

protected:
    // std::vector<SPtr<class Object>> Objects;
    // std::unordered_map<ObjectHandle, SPtr<class Object>> ObjectMap;
    std::unordered_map<Object*, SPtr<class Object>> ObjectMap;
public:
    virtual void Init();
    virtual void Start();
    virtual void Update(float DeltaTime);
    virtual void LateUpdate(float DeltaTime);
    virtual void Draw(FrameRenderInfo& RenderInfo);
    
    Level();
    virtual ~Level();

    virtual SPtr<Object> NewObject();

    SPtr<Object> GetObjectSPtr(Object* object);

    static Level* CurrentLevel;
    
    static LevelRegisterList* GlobalLevelRegisterList;
};



template<typename T>
struct StaticLevelRegister
{
    std::function<void(std::shared_ptr<Level>& CurrentLevel)> CreateLevel = [](std::shared_ptr<Level>& CurrentLevel)
    {
        CurrentLevel.reset();
        CurrentLevel = std::shared_ptr<Level>(new T());
        CurrentLevel->Init();
        CurrentLevel->Start();
    };
    
    StaticLevelRegister()
    {
        static_assert(std::is_base_of<Level, T>::value, "T must be derived from Level");

        if(Level::GlobalLevelRegisterList == nullptr)
        {
            Level::GlobalLevelRegisterList = new LevelRegisterList();
        }
        
        Level::GlobalLevelRegisterList->emplace_back(CreateLevel);
    }
};

#define LevelRegister(Level) static const StaticLevelRegister<Level> Level##LevelRegister;
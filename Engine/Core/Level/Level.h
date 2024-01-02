#pragma once
#include <unordered_map>
#include <vector>

#include "Engine/TypeDef.h"

class Object;
struct ObjectHandle;

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
    virtual void Draw();
    virtual ~Level();

    virtual SPtr<Object> NewObject();

    SPtr<Object> GetObjectSPtr(Object* object);

    static Level* CurrentLevel;
};

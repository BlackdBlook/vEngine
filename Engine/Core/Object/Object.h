#pragma once
#include <functional>
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Engine/TypeDef.h"


class SceneComponent;
class RenderInfo;
class Level;
class Component;


class Object : public std::enable_shared_from_this<Object>
{
protected:
    glm::vec3 pos;
    glm::quat rot;
    glm::vec3 scale;
    
    glm::mat4 model;

    SPtr<SceneComponent> rootComponent;

    std::vector<std::shared_ptr<Component>> Components;

public:
    bool needUpdateModelMat = true;
    Object();
    virtual void Start();
    virtual void Update(float DeltaTime);
    virtual void Draw(const RenderInfo& RenderInfo);

    template<typename T>
    SPtr<T> FindComponent(uint32 Index = 0);

    template<typename T>
    std::vector<SPtr<T>> FindComponents();

    template<typename T, typename ...Args>
    SPtr<T> CreateAttach(Args&&... args);
    
    virtual void Attach(std::shared_ptr<Component> Target);
    virtual void SetRootComponent(SPtr<SceneComponent> newRoot);
    template<typename T, typename ...Args>
    SPtr<T> CreateSetRootComponent(Args&&... args);
    
    virtual void Dettach(std::shared_ptr<Component> Target);

    
    virtual ~Object();
    virtual void SetPos(const glm::vec3& newPos);
    virtual glm::vec3 GetPos();
    virtual void SetRot(const glm::vec3& newRot);
    virtual void SetRot(const glm::quat& newRot);
    virtual void AddRot(const glm::vec3& newRot);
    virtual glm::quat GetRot();
    virtual void SetScale(const glm::vec3& newScale);
    virtual glm::vec3 GetScale();
    SceneComponent* GetRootComponent();

    const glm::mat4& GetModelMat();
    void LateUpdate(float delta_time);
};

template <typename T>
SPtr<T> Object::FindComponent(uint32 Index)
{
    uint32 i = 0;
    for(auto& c : Components)
    {
        SPtr<T> ret = std::dynamic_pointer_cast<T>(c);
        if(ret == nullptr)
        {
            continue;
        }

        if(i == Index)
        {
            return ret;
        }
        i++;
    }
    return nullptr;
}

template <typename T>
std::vector<SPtr<T>> Object::FindComponents()
{
    std::vector<SPtr<T>> ret;
    for(auto& c : Components)
    {
        SPtr<T> p = std::dynamic_pointer_cast<T>(c);
        if(p == nullptr)
        {
            continue;
        }
        
        ret.emplace_back(std::move(p));
    }
    return nullptr;
}

template <typename T, typename ... Args>
SPtr<T> Object::CreateAttach(Args&&... args)
{
    auto ret = std::make_shared<T>(args...);
    Attach(ret);
    return ret; 
}

template <typename T, typename ... Args>
SPtr<T> Object::CreateSetRootComponent(Args&&... args)
{
    static_assert(std::is_base_of<SceneComponent, T>::value,
        "只有SceneComponent能设置为Root");
    SPtr<T> ret = NewSPtr<T>(args...);
    SetRootComponent(ret);
    return ret;
}

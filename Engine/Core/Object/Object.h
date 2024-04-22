#pragma once
#include <functional>
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Engine/TypeDef.h"


class RenderInfo;
class Level;
class Component;


class Object
{
protected:
    glm::vec3 pos;
    glm::quat rot;
    glm::vec3 scale;
    
    glm::mat4 model;

    std::vector<std::shared_ptr<Component>> Components;

public:
    bool needUpdateModelMat = true;
    Object();
    virtual void Start();
    virtual void Update(float DeltaTime);
    virtual void Draw(const RenderInfo& RenderInfo);

    template<typename T, typename ...Args>
    SPtr<T> CreateAttach(Args&&... args)
    {
        auto ret = std::make_shared<T>(args...);
        Attach(ret);
        return ret; 
    }
    virtual void Attach(std::shared_ptr<Component> Target);
    
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

    const glm::mat4& GetModelMat();
    void LateUpdate(float delta_time);
};


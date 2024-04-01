#pragma once
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class RenderInfo;
class Level;
class Component;


class Object
{
protected:
    bool needUpdateModelMat = true;
    glm::vec3 pos;
    glm::quat rot;
    glm::vec3 scale;
    
    glm::mat4 model;

    std::vector<std::shared_ptr<Component>> Components;

public:
    Object();
    virtual void Start();
    virtual void Update(float DeltaTime);
    virtual void Draw(const RenderInfo& RenderInfo);

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

    glm::mat4 GetModelMat();
    void LateUpdate(float delta_time);
};


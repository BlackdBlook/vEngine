#pragma once
#include <functional>

#include "Engine/Core/Object/Object.h"

class Camera :public Object
{
    glm::mat4 viewmat;
    glm::mat4 projection;
    std::function<void(float)> updateFun;
    bool needUpdateView = true;
    Camera();
public:

    static Camera* GetCamera();

    void Update(float DeltaTime) override;
    void Attach(std::shared_ptr<Component> Target) override;
    void Dettach(std::shared_ptr<Component> Target) override;

    
    Camera(std::function<void(float)> update);
    void Reset(std::function<void(float)> update = [](float) {});
    glm::mat4 GetCameraView();
    void UpdateProjection();
    glm::mat4 GetCameraProjection();
    
    void SetPos(const glm::vec3& pos)override;
    void SetRot(const glm::vec3& newRot) override;
    void SetRot(const glm::quat& newRot) override;
    void AddRot(const glm::vec3& newRot) override;

    void SetLookAt(const glm::vec3& pos);
    
    /*glm::vec3 GetRot();
    void SetRot(glm::vec3 rot);*/

    glm::vec3 GetFont();
    glm::vec3 GetRight();
    glm::vec3 GetUp();
};


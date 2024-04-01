#pragma once
#include <functional>

#include "Engine/Core/Object/Object.h"

class Camera :public Object
{
    glm::mat4 viewmat;

    std::function<void(float)> updateFun;
    bool needUpdateView = true;
    static Camera* malloc();
public:

    void* operator new(size_t s)
    {
        //static Camera* cam = malloc();
        auto malloc = []() ->Camera*
        {
            void* cam = ::malloc(sizeof(Camera));
            if (cam == 0)
                return nullptr;
            memset(cam, 0, sizeof(Camera));
            Camera* ans = static_cast<Camera*>(cam);
            ans->Camera::Camera();
            return ans;
        };
        static Camera* cam = malloc();
        cam->Reset();
        return cam;
    }
    static Camera* GetCamera();

    virtual void Update(float DeltaTime) override;
    void Attach(std::shared_ptr<Component> Target) override;
    void Dettach(std::shared_ptr<Component> Target) override;

    Camera();
    Camera(std::function<void(float)> update);
    void Reset(std::function<void(float)> update = [](float) {});
    glm::mat4 GetCameraView();
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


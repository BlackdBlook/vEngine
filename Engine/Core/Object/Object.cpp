#include "Object.h"
#include "memory"
#include "Engine/Core/Component/Component.h"
#include "Engine/Core/Component/SceneComponent/SceneComponent.h"
#include "Engine/Core/Level/Level.h"
#include "Engine/Toolkit/math_utils.h"
#include "LogPrinter/Log.h"


Object::Object()
{
    ObjectTransform.pos = glm::vec3{0,0,0};
    ObjectTransform.rot = glm::vec3{0,0,0};
    ObjectTransform.scale = glm::vec3{1,1,1};
    GetModelMat();
    Components = {};
}

void Object::Start()
{
    
}

void Object::Update(float DeltaTime)
{
    for(auto& c : Components)
    {
        c->Update(DeltaTime);
    }
}

void Object::Draw(FrameRenderInfo& RenderInfo)
{
    for(auto& c : Components)
    {
        c->Draw(RenderInfo);
    }

    if(rootComponent)
    {
        rootComponent->Draw(RenderInfo);
    }
}

void Object::Attach(std::shared_ptr<Component> Target)
{
    if(Target->ParentObject != nullptr)
    {
        auto oldParent = Target->ParentObject;
        oldParent->Dettach(Target);
    }
    Target->ParentObject = this; 
    Components.emplace_back(Target);
    Target->OnAttached();
}

void Object::SetRootComponent(SPtr<SceneComponent> newRoot)
{
    if(newRoot->ParentComponent)
    {
        ERR("重复附加");
        return;
    }

    if(rootComponent)
    {
        newRoot->OnDettached();
        newRoot.reset();
    }
    
    rootComponent = newRoot;
    rootComponent->ParentComponent = nullptr;
    rootComponent->ParentObject = this;
    rootComponent->OnAttached();
}

void Object::Dettach(std::shared_ptr<Component> Target)
{
    auto it = Components.begin();
    
    while(it != Components.end())
    {
        if(*it == Target)
        {
            LOG(L"移除组件");
            Target->OnDettached();
            Components.erase(it);
            Target->ParentObject = nullptr;
            return;
        }
    }
    
    LOG(L"尝试移除不存在的组件");
    
}

Object::~Object()
{
    for(auto it : Components)
    {
        it->OnDettached();
        it->ParentObject = nullptr;
    }
    Components = {};
}

void Object::SetPos(const glm::vec3& newPos)
{
    ObjectTransform.pos = newPos;
    needUpdateModelMat = true;
}

glm::vec3 Object::GetPos()
{
    return ObjectTransform.pos;
}

void Object::SetRot(const glm::quat& newRot)
{
    ObjectTransform.rot = newRot;
    needUpdateModelMat = true;
}

void AddRotateToQuat(glm::quat& quat, const glm::vec3 axis, float angle)
{
    quat *= glm::angleAxis(glm::radians(angle), axis);
}

void Object::SetRot(const glm::vec3& newRot)
{
    // 传入应为角度制
    ObjectTransform.rot = glm::vec3{0};
    AddRotateToQuat(ObjectTransform.rot, glm::vec3{0,0,1}, newRot.z);
    AddRotateToQuat(ObjectTransform.rot, glm::vec3{0,1,0}, newRot.y);
    AddRotateToQuat(ObjectTransform.rot, glm::vec3{1,0,0}, newRot.x);
    
    needUpdateModelMat = true;
}

void Object::AddRot(const glm::vec3& newRot)
{
    AddRotateToQuat(ObjectTransform.rot, glm::vec3{0,0,1}, newRot.z);
    AddRotateToQuat(ObjectTransform.rot, glm::vec3{0,1,0}, newRot.y);
    AddRotateToQuat(ObjectTransform.rot, glm::vec3{1,0,0}, newRot.x);
    
    needUpdateModelMat = true;
}

glm::quat Object::GetRot()
{
    return ObjectTransform.rot;
}

void Object::SetScale(const glm::vec3& newScale)
{
    ObjectTransform.scale = newScale;
    needUpdateModelMat = true;
}

glm::vec3 Object::GetScale()
{
    return ObjectTransform.scale;
}

Transform Object::GetTransform()
{
    return ObjectTransform;
}

void Object::SetTransform(const Transform& transform)
{
    ObjectTransform = transform;
}

SceneComponent* Object::GetRootComponent()
{
    return rootComponent.get();
}

const glm::mat4& Object::GetModelMat()
{
    if(needUpdateModelMat)
    {
        MAT4(m);

        // 平移
        m = glm::translate(m, ObjectTransform.pos);

        // 旋转
        glm::mat4 rotationMatrix = glm::mat4_cast(ObjectTransform.rot);
        m *= rotationMatrix;

        // 缩放
        m = glm::scale(m, ObjectTransform.scale);

        this->model = m;

        if(rootComponent)
        {
            rootComponent->MarkRenderInfoDirty();
        }
    }

    return this->model;
}

void Object::LateUpdate(float delta_time)
{
    
}


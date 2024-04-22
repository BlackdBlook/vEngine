#include "Object.h"
#include "memory"
#include "Engine/Core/Component/Component.h"
#include "Engine/Core/Level/Level.h"
#include "Engine/Toolkit/math_utils.h"
#include "LogPrinter/Log.h"


Object::Object()
{
    pos = glm::vec3{0,0,0};
    rot = glm::vec3{0,0,0};
    scale = glm::vec3{1,1,1};
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

void Object::Draw(const RenderInfo& RenderInfo)
{
    for(auto& c : Components)
    {
        c->Draw(RenderInfo);
    }
}

void Object::Attach(std::shared_ptr<Component> Target)
{
    if(Target->Parent != nullptr)
    {
        auto oldParent = Target->Parent;
        oldParent->Dettach(Target);
    }
    Target->Parent = this; 
    Components.emplace_back(Target);
    Target->OnAttached();
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
            Target->Parent = nullptr;
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
        it->Parent = nullptr;
    }
    Components = {};
}

void Object::SetPos(const glm::vec3& newPos)
{
    pos = newPos;
    needUpdateModelMat = true;
}

glm::vec3 Object::GetPos()
{
    return pos;
}

void Object::SetRot(const glm::quat& newRot)
{
    rot = newRot;
    needUpdateModelMat = true;
}

void AddRotateToQuat(glm::quat& quat, const glm::vec3 axis, float angle)
{
    quat *= glm::angleAxis(glm::radians(angle), axis);
}

void Object::SetRot(const glm::vec3& newRot)
{
    // 传入应为角度制
    rot = glm::vec3{0};
    AddRotateToQuat(rot, glm::vec3{0,0,1}, newRot.z);
    AddRotateToQuat(rot, glm::vec3{0,1,0}, newRot.y);
    AddRotateToQuat(rot, glm::vec3{1,0,0}, newRot.x);
    
    needUpdateModelMat = true;
}

void Object::AddRot(const glm::vec3& newRot)
{
    AddRotateToQuat(rot, glm::vec3{0,0,1}, newRot.z);
    AddRotateToQuat(rot, glm::vec3{0,1,0}, newRot.y);
    AddRotateToQuat(rot, glm::vec3{1,0,0}, newRot.x);
    
    needUpdateModelMat = true;
}

glm::quat Object::GetRot()
{
    return rot;
}

void Object::SetScale(const glm::vec3& newScale)
{
    scale = newScale;
    needUpdateModelMat = true;
}

glm::vec3 Object::GetScale()
{
    return scale;
}

const glm::mat4& Object::GetModelMat()
{
    if(needUpdateModelMat)
    {
        MAT4(m);

        // 平移
        m = glm::translate(m, pos);

        // 旋转
        glm::mat4 rotationMatrix = glm::mat4_cast(rot);
        m *= rotationMatrix;

        // 缩放
        m = glm::scale(m, scale);

        this->model = m;
    }

    return this->model;
}

void Object::LateUpdate(float delta_time)
{
    
}


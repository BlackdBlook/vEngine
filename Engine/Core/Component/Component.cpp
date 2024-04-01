#include "Component.h"

#include "Engine/Core/Object/Object.h"
#include "LogPrinter/Log.h"


bool Component::operator==(Component* c)
{
    return false;
}

bool Component::operator==(const char* c)
{
    return false;
}

void Component::OnAttached()
{
}

void Component::OnDettached()
{
}

glm::vec3 Component::GetPos()
{
    if(Parent.expired())
    {
        LOG("Component not Attach");
        return VEC3_ZERO;
    }
    return Parent.lock()->GetPos();
}

glm::quat Component::GetRot()
{
    if(Parent.expired())
    {
        LOG("Component not Attach");
        return VEC3_ZERO;
    }
    return Parent.lock()->GetRot();
}

glm::vec3 Component::GetScale()
{
    if(Parent.expired())
    {
        LOG("Component not Attach");
        return VEC3_ZERO;
    }
    return Parent.lock()->GetScale();
}

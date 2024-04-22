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
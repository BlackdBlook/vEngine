#include "ObjectHandle.h"
#include "LogPrinter/Log.h"

uint64 ObjectHandle::HandleCounter = 0;

bool ObjectHandle::operator==(const ObjectHandle& other)
{
    return other.Handle == Handle;
}

ObjectHandle::ObjectHandle()
{
    Handle = HandleCounter;
    ++HandleCounter;
    if(HandleCounter - UINT64_MAX < 0xff)
    {
        LOG("ObjectHandle almost out of range!!!");
    }
}

ObjectHandle::ObjectHandle(const ObjectHandle& other)
{
    Handle = other.Handle;
}

bool ObjectHandle::IsVaild()
{
    return Handle != InvalidHandle;
}

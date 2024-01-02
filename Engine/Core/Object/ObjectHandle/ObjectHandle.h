#pragma once
#include <unordered_map>

#include "Engine/TypeDef.h"



struct ObjectHandle
{
    uint64 Handle;

    static uint64 HandleCounter;

    ObjectHandle();

    ObjectHandle(ObjectHandle&&) = delete;

    ObjectHandle(const ObjectHandle& other);

    bool IsVaild();

    bool operator==(const ObjectHandle& other);

    static constexpr uint64 InvalidHandle = UINT64_MAX;
};

namespace std
{
    template<>
    struct hash<ObjectHandle>
    {
        std::size_t operator()(const ObjectHandle& target)const noexcept
        {
            return hash<uint64>()(target.Handle);
        }
    };
}

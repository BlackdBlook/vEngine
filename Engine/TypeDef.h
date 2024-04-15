#pragma once
#include <memory>
#include <string>
#include <glm/glm.hpp>
//basic type
typedef short int16;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;
typedef float float32;
typedef double float64;
typedef char char8;
typedef char int8;
typedef unsigned char uchar8;
typedef unsigned char uint8;

// stl
using string  = std::string;

// shared_ptr
template<typename T>
using SPtr = std::shared_ptr<T>; 
template<typename T>
using WPtr = std::weak_ptr<T>;
template<typename T>
using UPtr = std::unique_ptr<T>;

template<typename T, typename ...Args>
inline SPtr<T> NewSPtr(Args&&... args)
{
    return std::make_shared<T>(args...);
}

#define MAT4(name) glm::mat4 name(1.0f)
#define LOG(...) Log::NameSpacePrinter(__FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...) Error::NameSpacePrinter(__FILE__, __LINE__, __VA_ARGS__)
#define VEC3_ZERO glm::vec3(0,0,0)



#pragma once
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <glm/detail/type_quat.hpp>

#include "Engine/TypeDef.h"

struct Transform
{
public:
    vec3 pos = vec3{0};
    quat rot = vec3{0};
    vec3 scale = vec3{1};
    
    Transform operator+(const Transform& other);
    Transform& operator+=(const Transform& other);
    Transform operator*(const Transform& other);
    Transform& operator*=(const Transform& other);

    glm::mat4 operator*(const glm::mat4& translate);

    glm::vec3 ToWorldPos(const glm::vec3& relativePos);

};
extern void operator*=(const Transform& transform, glm::mat4& translate);

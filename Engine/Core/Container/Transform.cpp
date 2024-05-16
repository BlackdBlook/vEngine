#include "Transform.h"

#include "Engine/Toolkit/math_utils.h"

Transform Transform::operator+(const Transform& other)
{
    Transform result;
    result.pos = this->pos + other.pos;
    result.rot = this->rot * other.rot;
    result.scale = this->scale + other.scale;
    return result;
}

Transform& Transform::operator+=(const Transform& other)
{
    pos = this->pos + other.pos;
    rot = this->rot * other.rot;
    scale = this->scale + other.scale;
    return *this;
}

Transform Transform::operator*(const Transform& other)
{
    Transform result;
    result.scale = this->scale * other.scale; // 缩放的组合
    result.rot = this->rot * other.rot; // 旋转的组合
    result.pos = this->pos + this->rot * (this->scale * other.pos); // 位置的组合
    return result;
}

Transform& Transform::operator*=(const Transform& other)
{
    scale = this->scale * other.scale; // 缩放的组合
    rot = this->rot * other.rot; // 旋转的组合
    pos = this->pos + this->rot * (this->scale * other.pos); // 位置的组合
    return *this;
}

glm::mat4 Transform::operator*(const glm::mat4& translate)
{
    glm::mat4 m = translate;

    // 平移
    m = glm::translate(m, pos);

    // 旋转
    glm::mat4 rotationMatrix = glm::mat4_cast(rot);
    m *= rotationMatrix;

    // 缩放
    m = glm::scale(m, scale);

    return m;
}

glm::vec3 Transform::ToWorldPos(const glm::vec3& relativePos)
{
    // 将相对位置旋转到世界坐标系
    glm::vec3 rotatedPos = rot * relativePos;
    // 加上当前变换的位置
    glm::vec3 worldPos = pos + rotatedPos;
    return worldPos;
}

void operator*=(const Transform& transform, glm::mat4& translate)
{
    // 平移
    translate = glm::translate(translate, transform.pos);

    // 旋转
    glm::mat4 rotationMatrix = glm::mat4_cast(transform.rot);
    translate *= rotationMatrix;

    // 缩放
    translate = glm::scale(translate, transform.scale);
}

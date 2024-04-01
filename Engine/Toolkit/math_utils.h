#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>


class QuaternionUtils {
public:
    // 从前向量得到四元数
    static glm::quat quatFromForwardVector(const glm::vec3& forward);

    // 从四元数得到前向量
    static glm::vec3 forwardVectorFromQuat(const glm::quat& q);

    // 从欧拉角得到四元数
    static glm::quat quatFromEulerAngles(const glm::vec3& eulerAngles);

    // 从四元数得到欧拉角
    static glm::vec3 eulerAnglesFromQuat(const glm::quat& q);
};


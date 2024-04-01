#include "math_utils.h"

glm::quat QuaternionUtils::quatFromForwardVector(const glm::vec3& forward)
{
    return glm::quatLookAt(glm::normalize(forward), glm::vec3(0, 1, 0));
}

glm::vec3 QuaternionUtils::forwardVectorFromQuat(const glm::quat& q)
{
    return glm::normalize(glm::mat3_cast(q) * glm::vec3(0, 0, -1));
}

glm::quat QuaternionUtils::quatFromEulerAngles(const glm::vec3& eulerAngles)
{
    return glm::quat(eulerAngles);
}

glm::vec3 QuaternionUtils::eulerAnglesFromQuat(const glm::quat& q)
{
    return glm::eulerAngles(q);
}
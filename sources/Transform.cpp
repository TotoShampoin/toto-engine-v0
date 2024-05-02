#include "TotoEngine/Math/Transform.hpp"
#include "TotoEngine/Math/Primitives.hpp"
#include <glm/geometric.hpp>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace TotoEngine {

void Math::Transform::translate(const Math::Vector3& translation) {
    _position += translation;
}
void Math::Transform::rotate(const float& angle, const Math::Vector3& axis) {
    auto rotated = glm::rotate(rotationMatrix(), angle, axis);
    glm::extractEulerAngleXYZ(rotated, _rotation.x, _rotation.y, _rotation.z);
}
void Math::Transform::scale(const Math::Vector3& factor) {
    _scale *= factor;
}
void Math::Transform::lookAt(const Math::Vector3& target, const Math::Vector3& up) {
    if(target == _position)
        return;
    auto direction = glm::normalize(target - _position);
    auto up_dir = glm::normalize(up);
    if(glm::abs(glm::dot(direction, up_dir)) > .99f) {
        _rotation.y = std::atan2(up.x, up.z);
        _rotation.x = std::atan2(up.y, std::sqrt(up.x * up.x + up.z * up.z));
        return;
    }
    auto rotation = glm::inverse(glm::lookAt(_position, target, up));
    glm::extractEulerAngleXYZ(rotation, _rotation.x, _rotation.y, _rotation.z);
}

Math::Matrix4 Math::Transform::matrix() const {
    return translationMatrix() * rotationMatrix() * scaleMatrix();
}
Math::Matrix4 Math::Transform::translationMatrix() const {
    return glm::translate(glm::mat4(1.0f), _position);
}
Math::Matrix4 Math::Transform::rotationMatrix() const {
    return glm::rotate(glm::mat4(1.0f), _rotation.x, {1.0f, 0.0f, 0.0f}) *
           glm::rotate(glm::mat4(1.0f), _rotation.y, {0.0f, 1.0f, 0.0f}) *
           glm::rotate(glm::mat4(1.0f), _rotation.z, {0.0f, 0.0f, 1.0f});
}
Math::Matrix4 Math::Transform::scaleMatrix() const {
    return glm::scale(glm::mat4(1.0f), _scale);
}

}

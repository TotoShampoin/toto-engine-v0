#include "TotoEngine/Transform.hpp"
#include "TotoEngine/Primitives.hpp"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace TotoEngine {

void Transform::translate(const Vector3& translation) {
    _position += translation;
}
void Transform::rotate(const float& angle, const Vector3& axis) {
    auto rotated = glm::rotate(rotationMatrix(), angle, axis);
    glm::extractEulerAngleXYZ(rotated, _rotation.x, _rotation.y, _rotation.z);
}
void Transform::scale(const Vector3& factor) {
    _scale *= factor;
}
void Transform::lookAt(const Vector3& target, const Vector3& up) {
    if(target == _position)
        return;
    if(target - _position == up) {
        _rotation.y = std::atan2(up.x, up.z);
        _rotation.x = std::atan2(up.y, std::sqrt(up.x * up.x + up.z * up.z));
        return;
    }
    auto rotation = glm::lookAt(_position, target, up);
    glm::extractEulerAngleXYZ(rotation, _rotation.x, _rotation.y, _rotation.z);
}

Matrix4 Transform::matrix() const {
    return translationMatrix() * rotationMatrix() * scaleMatrix();
}
Matrix4 Transform::translationMatrix() const {
    return glm::translate(glm::mat4(1.0f), _position);
}
Matrix4 Transform::rotationMatrix() const {
    return glm::rotate(glm::mat4(1.0f), _rotation.x, {1.0f, 0.0f, 0.0f}) *
           glm::rotate(glm::mat4(1.0f), _rotation.y, {0.0f, 1.0f, 0.0f}) *
           glm::rotate(glm::mat4(1.0f), _rotation.z, {0.0f, 0.0f, 1.0f});
}
Matrix4 Transform::scaleMatrix() const {
    return glm::scale(glm::mat4(1.0f), _scale);
}

}

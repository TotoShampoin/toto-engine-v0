#include "TotoEngine/Math/Transform.hpp"
#include "TotoEngine/Math/Primitives.hpp"
#include <glm/geometric.hpp>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace TotoEngine {

namespace Math {

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

Transform Transform::fromMatrix(const Matrix4& matrix) {
    Transform transform;
    glm::extractEulerAngleXYZ(matrix, transform._rotation.x, transform._rotation.y, transform._rotation.z);
    transform._position = matrix[3];
    transform._scale = glm::vec3(glm::length(matrix[0]), glm::length(matrix[1]), glm::length(matrix[2]));
    return transform;
}

Transform Transform::applied(const Transform& other) const {
    return Transform::fromMatrix(matrix() * other.matrix());
}
Transform Transform::inverse() const {
    return Transform::fromMatrix(inverseMatrix());
}

Matrix4 Transform::matrix() const {
    return translationMatrix() * rotationMatrix() * scaleMatrix();
}
Matrix4 Transform::inverseMatrix() const {
    return glm::inverse(matrix());
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

}

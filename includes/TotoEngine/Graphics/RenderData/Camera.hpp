#pragma once

#include <TotoEngine/Math/Primitives.hpp>
#include <TotoEngine/Math/Transform.hpp>
#include <glm/matrix.hpp>

namespace TotoEngine {

namespace Graphics {

class Camera {
public:
    Camera(const Math::Matrix4& projection_matrix):
        _projection_matrix(projection_matrix)
    {}

    Math::Matrix4 viewMatrix() const {
        return _transform.inverseMatrix();
    }
    Math::Matrix3 viewNormalMatrix() const {
        return Math::Matrix3(glm::transpose(matrix()));
    }

    Math::Vector2 worldToScreen(const Math::Vector3& point) const {
        auto projected_point = projectionMatrix() * viewMatrix() * Math::Vector4(point, 1);
        return Math::Vector2(projected_point.x, -projected_point.y) / projected_point.w * 0.5f + 0.5f;
    }

    Math::Matrix4& projectionMatrix() { return _projection_matrix; }
    const Math::Matrix4& projectionMatrix() const { return _projection_matrix; }

    void translate(const Math::Vector3& translation) { return _transform.translate(translation); };
    void rotate(const float& angle, const Math::Vector3& axis) { return _transform.rotate(angle, axis); };
    void lookAt(const Math::Vector3& target, const Math::Vector3& up = {0.0f, 1.0f, 0.0f}) { return _transform.lookAt(target, up); };
    Math::Vector3& position() { return _transform.position(); };
    Math::Vector3& rotation() { return _transform.rotation(); };
    Math::Vector3 position() const { return _transform.position(); };
    Math::Vector3 rotation() const { return _transform.rotation(); };
    Math::Matrix4 matrix() const { return _transform.matrix(); };
    Math::Matrix4 translationMatrix() const { return _transform.translationMatrix(); };
    Math::Matrix4 rotationMatrix() const { return _transform.rotationMatrix(); };
private:
    Math::Matrix4 _projection_matrix;
    Math::Transform _transform;
};

}

}

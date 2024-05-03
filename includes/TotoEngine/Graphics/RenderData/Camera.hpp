#pragma once

#include <TotoEngine/Math/Primitives.hpp>
#include <TotoEngine/Math/Transform.hpp>
#include <glm/matrix.hpp>

namespace TotoEngine {

namespace Graphics {

class Camera : private Math::Transform {
public:
    Camera(const Math::Matrix4& projection_matrix):
        _projection_matrix(projection_matrix)
    {}

    Math::Matrix4 viewMatrix() const {
        return inverseMatrix();
    }
    Math::Matrix3 viewNormalMatrix() const {
        return Math::Matrix3(glm::transpose(matrix()));
    }

    Math::Matrix4& projectionMatrix() { return _projection_matrix; }
    const Math::Matrix4& projectionMatrix() const { return _projection_matrix; }

    using Math::Transform::translate;
    using Math::Transform::rotate;
    using Math::Transform::lookAt;
    using Math::Transform::matrix;
    using Math::Transform::position;
    using Math::Transform::rotation;
    using Math::Transform::translationMatrix;
    using Math::Transform::rotationMatrix;
private:
    Math::Matrix4 _projection_matrix;
};

}

}

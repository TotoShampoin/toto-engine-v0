#pragma once

#include "TotoEngine/Primitives.hpp"
#include "TotoEngine/Transform.hpp"
#include <glm/matrix.hpp>

namespace TotoEngine {

class Camera : public Transformed {
public:
    Camera(const Matrix4& projection_matrix):
        _projection_matrix(projection_matrix)
    {}

    Matrix4 viewMatrix() const {
        return glm::inverse(matrix());
    }
    Matrix3 viewNormalMatrix() const {
        return Matrix3(glm::transpose(matrix()));
    }

    Matrix4& projectionMatrix() { return _projection_matrix; }
    const Matrix4& projectionMatrix() const { return _projection_matrix; }
private:
    Matrix4 _projection_matrix;
};

}

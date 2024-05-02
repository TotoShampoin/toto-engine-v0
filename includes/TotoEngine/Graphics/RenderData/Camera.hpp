#pragma once

#include <TotoEngine/Math/Primitives.hpp>
#include <TotoEngine/Math/Transform.hpp>
#include <glm/matrix.hpp>

namespace TotoEngine {

namespace Graphics {

class Camera : public Math::Transformed {
public:
    Camera(const Math::Matrix4& projection_matrix):
        _projection_matrix(projection_matrix)
    {}

    Math::Matrix4 viewMatrix() const {
        return glm::inverse(matrix());
    }
    Math::Matrix3 viewNormalMatrix() const {
        return Math::Matrix3(glm::transpose(matrix()));
    }

    Math::Matrix4& projectionMatrix() { return _projection_matrix; }
    const Math::Matrix4& projectionMatrix() const { return _projection_matrix; }
private:
    Math::Matrix4 _projection_matrix;
};

}

}

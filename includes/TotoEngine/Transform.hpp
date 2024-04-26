#pragma once

#include "Primitives.hpp"

namespace TotoEngine {

class Transform {
public:
    void translate(const Vector3&);
    void rotate(const float&, const Vector3&);
    void scale(const Vector3&);
    void lookAt(const Vector3&, const Vector3& up = {0.0f, 1.0f, 0.0f});

    Matrix4 matrix() const;
    Matrix4 translationMatrix() const;
    Matrix4 rotationMatrix() const;
    Matrix4 scaleMatrix() const;

    Vector3& position() { return _position; }
    Vector3& rotation() { return _rotation; }
    Vector3& scale() { return _scale; }

    Vector3 position() const { return _position; }
    Vector3 rotation() const { return _rotation; }
    Vector3 scale() const { return _scale; }
private:
    Vector3 _position {0.0f, 0.0f, 0.0f};
    Vector3 _rotation {0.0f, 0.0f, 0.0f};
    Vector3 _scale {1.0f, 1.0f, 1.0f};
};

};

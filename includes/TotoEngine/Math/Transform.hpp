#pragma once

#include <TotoEngine/Math/Primitives.hpp>

namespace TotoEngine {

namespace Math {

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

class Transformed {
public:
    void translate(const Vector3& translation) { _transform.translate(translation); }
    void rotate(const float& angle, const Vector3& axis) { _transform.rotate(angle, axis); }
    void scale(const Vector3& scale) { _transform.scale(scale); }
    void lookAt(const Vector3& target, const Vector3& up = {0.0f, 1.0f, 0.0f}) { _transform.lookAt(target, up); }

    Matrix4 matrix() const { return _transform.matrix(); }
    Matrix4 translationMatrix() const { return _transform.translationMatrix(); }
    Matrix4 rotationMatrix() const { return _transform.rotationMatrix(); }
    Matrix4 scaleMatrix() const { return _transform.scaleMatrix(); }

    Vector3& position() { return _transform.position(); }
    Vector3& rotation() { return _transform.rotation(); }
    Vector3& scale() { return _transform.scale(); }

    Vector3 position() const { return _transform.position(); }
    Vector3 rotation() const { return _transform.rotation(); }
    Vector3 scale() const { return _transform.scale(); }

    Transform& transform() { return _transform; }
    const Transform& transform() const { return _transform; }
private:
    Transform _transform;
};

}

};

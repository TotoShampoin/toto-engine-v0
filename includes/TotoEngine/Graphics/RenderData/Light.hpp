#pragma once

#include <TotoEngine/Math/Primitives.hpp>
#include <TotoEngine/Math/Transform.hpp>

namespace TotoEngine {

namespace Graphics {

enum class LightType {
    AMBIENT = 0,
    POINT = 1,
    DIRECTIONAL = 2,
    SPOT = 3,
    AREA = 4,
};

class Light {
public:
    Light(
        const LightType& type = LightType::POINT, const Math::ColorRGB& color = Math::ColorRGB(1.0f, 1.0f, 1.0f),
        const float& intensity = 1.0f
    )
        : _type(type),
          _color(color),
          _intensity(intensity) {}

    [[nodiscard]] glm::vec3 direction() const { return rotationMatrix() * Math::Vector4(0.0f, 0.0f, 1.0f, 0.0f); }

    [[nodiscard]] LightType& type() { return _type; }
    [[nodiscard]] Math::ColorRGB& color() { return _color; }
    [[nodiscard]] float& intensity() { return _intensity; }

    [[nodiscard]] LightType type() const { return _type; }
    [[nodiscard]] Math::ColorRGB color() const { return _color; }
    [[nodiscard]] float intensity() const { return _intensity; }

    void translate(const Math::Vector3& translation) { return _transform.translate(translation); };
    void rotate(const float& angle, const Math::Vector3& axis) { return _transform.rotate(angle, axis); };
    void lookAt(const Math::Vector3& target, const Math::Vector3& up = {0.0f, 1.0f, 0.0f}) {
        return _transform.lookAt(target, up);
    };
    Math::Vector3& position() { return _transform.position(); };
    Math::Vector3& rotation() { return _transform.rotation(); };
    Math::Vector3 position() const { return _transform.position(); };
    Math::Vector3 rotation() const { return _transform.rotation(); };
    Math::Matrix4 matrix() const { return _transform.matrix(); };
    Math::Matrix4 translationMatrix() const { return _transform.translationMatrix(); };
    Math::Matrix4 rotationMatrix() const { return _transform.rotationMatrix(); };

private:
    LightType _type;
    Math::ColorRGB _color;
    float _intensity;
    Math::Transform _transform;
};

} // namespace Graphics

} // namespace TotoEngine

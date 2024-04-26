#pragma once

#include "TotoEngine/Graphics/ShaderProgram.hpp"
#include <TotoEngine/Primitives.hpp>

namespace TotoEngine {

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
        const LightType& type = LightType::POINT,
        const ColorRGB& color = ColorRGB(1.0f, 1.0f, 1.0f),
        const float& intensity = 1.0f
    ) : _type(type), _color(color), _intensity(intensity) {}

    void apply(ShaderProgram& program, const size_t& index = 0) const;

    [[nodiscard]] LightType& type() { return _type; }
    [[nodiscard]] ColorRGB& color() { return _color; }
    [[nodiscard]] float& intensity() { return _intensity; }
    [[nodiscard]] Vector3& position() { return _position; }
    [[nodiscard]] Vector3& direction() { return _direction; }

    [[nodiscard]] LightType type() const { return _type; }
    [[nodiscard]] ColorRGB color() const { return _color; }
    [[nodiscard]] float intensity() const { return _intensity; }
    [[nodiscard]] Vector3 position() const { return _position; }
    [[nodiscard]] Vector3 direction() const { return _direction; }
private:
    LightType _type;
    ColorRGB _color;
    float _intensity;
    Vector3 _position {0};
    Vector3 _direction {0, 0, -1};
};

}

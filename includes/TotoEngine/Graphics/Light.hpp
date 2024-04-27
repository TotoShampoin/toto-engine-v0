#pragma once

#include "TotoEngine/Transform.hpp"
#include <TotoEngine/Primitives.hpp>

namespace TotoEngine {

enum class LightType {
    AMBIENT = 0,
    POINT = 1,
    DIRECTIONAL = 2,
    SPOT = 3,
    AREA = 4,
};

class Light : public Transformed {
public:
    Light(
        const LightType& type = LightType::POINT,
        const ColorRGB& color = ColorRGB(1.0f, 1.0f, 1.0f),
        const float& intensity = 1.0f
    ) : _type(type), _color(color), _intensity(intensity) {}

    [[nodiscard]] LightType& type() { return _type; }
    [[nodiscard]] ColorRGB& color() { return _color; }
    [[nodiscard]] float& intensity() { return _intensity; }

    [[nodiscard]] LightType type() const { return _type; }
    [[nodiscard]] ColorRGB color() const { return _color; }
    [[nodiscard]] float intensity() const { return _intensity; }
private:
    LightType _type;
    ColorRGB _color;
    float _intensity;
};

}

#pragma once

#include <TotoEngine/Math/Transform.hpp>
#include <TotoEngine/Math/Primitives.hpp>

namespace TotoEngine {

namespace Graphics {

enum class LightType {
    AMBIENT = 0,
    POINT = 1,
    DIRECTIONAL = 2,
    SPOT = 3,
    AREA = 4,
};

class Light : public Math::Transformed {
public:
    Light(
        const LightType& type = LightType::POINT,
        const Math::ColorRGB& color = Math::ColorRGB(1.0f, 1.0f, 1.0f),
        const float& intensity = 1.0f
    ) : _type(type), _color(color), _intensity(intensity) {}

    [[nodiscard]] LightType& type() { return _type; }
    [[nodiscard]] Math::ColorRGB& color() { return _color; }
    [[nodiscard]] float& intensity() { return _intensity; }

    [[nodiscard]] LightType type() const { return _type; }
    [[nodiscard]] Math::ColorRGB color() const { return _color; }
    [[nodiscard]] float intensity() const { return _intensity; }
private:
    LightType _type;
    Math::ColorRGB _color;
    float _intensity;
};

}

}

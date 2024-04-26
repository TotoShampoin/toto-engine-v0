#include "TotoEngine/Graphics/Light.hpp"
#include <format>

namespace TotoEngine {

void Light::apply(ShaderProgram& program, const size_t& index) const {
    program.uniform(std::format("u_lights[{}].color", index), _color);
    program.uniform(std::format("u_lights[{}].intensity", index), _intensity);
    program.uniform(std::format("u_lights[{}].type", index), static_cast<int>(_type));
    if(_type == LightType::POINT) {
        program.uniform(std::format("u_lights[{}].pos_or_dir", index), _position);
    }
    else if(_type == LightType::DIRECTIONAL) {
        program.uniform(std::format("u_lights[{}].pos_or_dir", index), _direction);
    }
}

}

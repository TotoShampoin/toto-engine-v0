#include "TotoEngine/Graphics/Materials.hpp"

#include "../res/shaders/vertex.glsl.hpp"
#include "../res/shaders/basic_fragment.glsl.hpp"
#include "TotoEngine/Graphics/ShaderFile.hpp"
#include "TotoEngine/Graphics/ShaderProgram.hpp"
#include "TotoEngine/Graphics/Texture.hpp"

namespace TotoEngine {

ShaderProgram& BasicMaterial::shader() {
    static auto shader = ShaderProgram(
        VertexShaderFile(vertex),
        FragmentShaderFile(basic_fragment)
    );
    return shader;
}

void BasicMaterial::apply() {
    shader().uniform("u_color", color);
    shader().uniform("u_opacity", opacity);
    if(map.has_value()) {
        glActiveTexture(GL_TEXTURE0);
        Texture2D::bind(map.value());
        shader().uniform("u_map", 0);
    }
    shader().uniform("u_use_map", map.has_value());
}

}

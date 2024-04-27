#include "TotoEngine/Graphics/Materials.hpp"

#include "../impl/shaders/basic.vert.hpp"
#include "../impl/shaders/basic.frag.hpp"
#include "../impl/shaders/phong.frag.hpp"

namespace TotoEngine {

ShaderProgram& BasicMaterial::shader() const {
    static auto shader = ShaderProgram(
        VertexShaderFile(basic_vert),
        FragmentShaderFile(basic_frag)
    );
    return shader;
}

void BasicMaterial::apply() const {
    shader().uniform("u_color", color);
    shader().uniform("u_opacity", opacity);
    if(map.has_value()) {
        glActiveTexture(GL_TEXTURE0);
        Texture2D::bind(map);
        shader().uniform("u_map", 0);
    }
    shader().uniform("u_use_map", map.has_value());
}

ShaderProgram& PhongMaterial::shader() const {
    static auto shader = ShaderProgram(
        VertexShaderFile(basic_vert),
        FragmentShaderFile(phong_frag)
    );
    return shader;
}

void PhongMaterial::apply() const {
    shader().uniform("u_ambient", ambient);
    shader().uniform("u_diffuse", diffuse);
    shader().uniform("u_specular", specular);
    shader().uniform("u_emissive", emissive);
    shader().uniform("u_shininess", shininess);
    shader().uniform("u_opacity", opacity);
    if(ambient_map.has_value()) {
        glActiveTexture(GL_TEXTURE0);
        Texture2D::bind(ambient_map);
        shader().uniform("u_ambient_map", 0);
    }
    if(diffuse_map.has_value()) {
        glActiveTexture(GL_TEXTURE1);
        Texture2D::bind(diffuse_map);
        shader().uniform("u_diffuse_map", 1);
    }
    if(specular_map.has_value()) {
        glActiveTexture(GL_TEXTURE2);
        Texture2D::bind(specular_map);
        shader().uniform("u_specular_map", 2);
    }
    if(emissive_map.has_value()) {
        glActiveTexture(GL_TEXTURE3);
        Texture2D::bind(emissive_map);
        shader().uniform("u_emissive_map", 3);
    }
    if(shininess_map.has_value()) {
        glActiveTexture(GL_TEXTURE4);
        Texture2D::bind(shininess_map);
        shader().uniform("u_shininess_map", 4);
    }
    if(opacity_map.has_value()) {
        glActiveTexture(GL_TEXTURE5);
        Texture2D::bind(opacity_map);
        shader().uniform("u_opacity_map", 5);
    }
    shader().uniform("u_use_ambient_map", ambient_map.has_value());
    shader().uniform("u_use_diffuse_map", diffuse_map.has_value());
    shader().uniform("u_use_specular_map", specular_map.has_value());
    shader().uniform("u_use_emissive_map", emissive_map.has_value());
    shader().uniform("u_use_shininess_map", shininess_map.has_value());
    shader().uniform("u_use_opacity_map", opacity_map.has_value());
}

}

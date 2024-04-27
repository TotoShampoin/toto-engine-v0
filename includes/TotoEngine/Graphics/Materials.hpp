#pragma once

#include <TotoEngine/Primitives.hpp>
#include <TotoEngine/Graphics/ShaderProgram.hpp>
#include <TotoEngine/Graphics/Texture.hpp>

#include <optional>

namespace TotoEngine {

class BasicMaterial {
public:
    ColorRGB color { 1.0f, 1.0f, 1.0f };
    float opacity { 1.0f };
    optional_ref<Texture2D> map { std::nullopt };

    void apply() const;
    ShaderProgram& shader() const;
};

class PhongMaterial {
public:
    ColorRGB ambient { 1.0f, 1.0f, 1.0f };
    ColorRGB diffuse { 1.0f, 1.0f, 1.0f };
    ColorRGB specular { 1.0f, 1.0f, 1.0f };
    ColorRGB emissive { 0.0f, 0.0f, 0.0f };
    float shininess { 32.0f };
    float opacity { 1.0f };

    optional_ref<Texture2D> ambient_map { std::nullopt };
    optional_ref<Texture2D> diffuse_map { std::nullopt };
    optional_ref<Texture2D> specular_map { std::nullopt };
    optional_ref<Texture2D> emissive_map { std::nullopt };
    optional_ref<Texture2D> shininess_map { std::nullopt };
    optional_ref<Texture2D> opacity_map { std::nullopt };

    void apply() const;
    ShaderProgram& shader() const;
};

class PBRMaterial {
public:
    ColorRGB albedo { 1.0f, 1.0f, 1.0f };
    float metallic { 0.0f };
    float roughness { 1.0f };
    float ao { 1.0f };
    float opacity { 1.0f };

    optional_ref<Texture2D> albedo_map { std::nullopt };
    optional_ref<Texture2D> metallic_map { std::nullopt };
    optional_ref<Texture2D> roughness_map { std::nullopt };
    optional_ref<Texture2D> ao_map { std::nullopt };
    optional_ref<Texture2D> opacity_map { std::nullopt };
    optional_ref<Texture2D> normal_map { std::nullopt };

    void apply() const;
    ShaderProgram& shader() const;
};

}

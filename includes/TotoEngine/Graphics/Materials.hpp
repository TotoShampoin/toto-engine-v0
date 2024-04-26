#pragma once

#include <optional>

#include "Aliases.hpp"
#include "ShaderProgram.hpp"
#include "Texture.hpp"

namespace TotoEngine {

class Material {
public:
    virtual void apply() = 0;
};

class BasicMaterial : public Material {
public:
    ColorRGB color { 1.0f, 1.0f, 1.0f };
    float opacity { 1.0f };
    std::optional<Texture2D> map { std::nullopt };

    void apply() override;
    static ShaderProgram& shader();
};
class PhongMaterial : public Material {
public:
    ColorRGB ambient { 0.0f, 0.0f, 0.0f };
    ColorRGB diffuse { 1.0f, 1.0f, 1.0f };
    ColorRGB specular { 1.0f, 1.0f, 1.0f };
    ColorRGB emissive { 0.0f, 0.0f, 0.0f };
    float shininess { 32.0f };
    float opacity { 1.0f };

    std::optional<Texture2D> ambient_map { std::nullopt };
    std::optional<Texture2D> diffuse_map { std::nullopt };
    std::optional<Texture2D> specular_map { std::nullopt };
    std::optional<Texture2D> emissive_map { std::nullopt };
    std::optional<Texture2D> shininess_map { std::nullopt };
    std::optional<Texture2D> opacity_map { std::nullopt };

    void apply() override;
    static ShaderProgram& shader();
};
class PBRMaterial : public Material {
public:
    ColorRGB albedo { 1.0f, 1.0f, 1.0f };
    float metallic { 0.0f };
    float roughness { 1.0f };
    float ao { 1.0f };
    float opacity { 1.0f };

    std::optional<Texture2D> albedo_map { std::nullopt };
    std::optional<Texture2D> metallic_map { std::nullopt };
    std::optional<Texture2D> roughness_map { std::nullopt };
    std::optional<Texture2D> ao_map { std::nullopt };
    std::optional<Texture2D> opacity_map { std::nullopt };

    void apply() override;
    static ShaderProgram& shader();
};

}

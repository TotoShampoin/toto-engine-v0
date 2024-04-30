#pragma once

#include <TotoEngine/Primitives.hpp>
#include <TotoEngine/Graphics/ShaderProgram.hpp>
#include <TotoEngine/Graphics/Texture.hpp>

#include <functional>
#include <optional>
#include <variant>

namespace TotoEngine {

class BasicMaterial {
public:
    ColorRGB color { 1.0f, 1.0f, 1.0f };
    float opacity { 1.0f };
    optional_ref<Texture2D> map { std::nullopt };

    void apply(ShaderProgram& = shader()) const;
    static ShaderProgram& shader();
};

class PositionMaterial {
public:
    void apply(ShaderProgram& = shader()) const;
    static ShaderProgram& shader();
};
class DepthMaterial {
public:
    void apply(ShaderProgram& = shader()) const;
    static ShaderProgram& shader();
};
class NormalMaterial {
public:
    optional_ref<Texture2D> map { std::nullopt };

    void apply(ShaderProgram& = shader()) const;
    static ShaderProgram& shader();
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

    void apply(ShaderProgram& = shader()) const;
    static ShaderProgram& shader();
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

    void apply(ShaderProgram& = shader()) const;
    static ShaderProgram& shader();
};

struct Material {
    std::variant<
        std::reference_wrapper<BasicMaterial>,
        std::reference_wrapper<PositionMaterial>,
        std::reference_wrapper<DepthMaterial>,
        std::reference_wrapper<NormalMaterial>,
        std::reference_wrapper<PhongMaterial>
    > data;

    void apply(ShaderProgram& shader) const {
        std::visit([&](auto&& material) {
            material.get().apply(shader);
        }, data);
    }
    ShaderProgram& shader() {
        return std::visit([](auto&& material) -> ShaderProgram& {
            return material.get().shader();
        }, data);
    }
};

}

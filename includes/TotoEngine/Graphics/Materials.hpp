#pragma once

#include <TotoEngine/Primitives.hpp>
#include <TotoEngine/Graphics/ShaderProgram.hpp>
#include <TotoEngine/Graphics/Texture.hpp>

#include <functional>
#include <optional>

namespace TotoEngine {

class BasicMaterial {
public:
    BasicMaterial() { (void)shader(); }

    ColorRGB color { 1.0f, 1.0f, 1.0f };
    float opacity { 1.0f };
    optional_ref<Texture2D> map { std::nullopt };

    void apply() const;
    ShaderProgram& shader() const;
};

class PositionMaterial {
public:
    PositionMaterial() { (void)shader(); }

    void apply() const;
    ShaderProgram& shader() const;
};
class DepthMaterial {
public:
    DepthMaterial() { (void)shader(); }

    void apply() const;
    ShaderProgram& shader() const;
};
class NormalMaterial {
public:
    NormalMaterial() { (void)shader(); }

    optional_ref<Texture2D> map { std::nullopt };

    void apply() const;
    ShaderProgram& shader() const;
};

class PhongMaterial {
public:
    PhongMaterial() { (void)shader(); }

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
    PBRMaterial() { (void)shader(); }

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

struct Material {
    std::variant<
        std::reference_wrapper<BasicMaterial>,
        std::reference_wrapper<PositionMaterial>,
        std::reference_wrapper<DepthMaterial>,
        std::reference_wrapper<NormalMaterial>,
        std::reference_wrapper<PhongMaterial>
    > data;

    void apply() const {
        std::visit([](auto&& material) { material.get().apply(); }, data);
    }
    ShaderProgram& shader() const {
        return std::visit([](auto&& material) -> ShaderProgram& { return material.get().shader(); }, data);
    }
};

}

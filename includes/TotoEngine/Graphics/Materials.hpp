#pragma once

#include <optional>

#include "Aliases.hpp"
#include "Texture.hpp"

namespace TotoEngine {

struct BasicMaterial {
    ColorRGB color;
    float opacity;

    std::optional<Texture2D> map;
};
struct PhongMaterial {
    ColorRGB ambient;
    ColorRGB diffuse;
    ColorRGB specular;
    ColorRGB emissive;
    float shininess;
    float opacity;

    std::optional<Texture2D> ambient_map;
    std::optional<Texture2D> diffuse_map;
    std::optional<Texture2D> specular_map;
    std::optional<Texture2D> emissive_map;
    std::optional<Texture2D> shininess_map;
    std::optional<Texture2D> opacity_map;
};
struct PBRMaterial {
    ColorRGB albedo;
    float metallic;
    float roughness;
    float ao;
    float opacity;

    std::optional<Texture2D> albedo_map;
    std::optional<Texture2D> metallic_map;
    std::optional<Texture2D> roughness_map;
    std::optional<Texture2D> ao_map;
    std::optional<Texture2D> opacity_map;
};

}

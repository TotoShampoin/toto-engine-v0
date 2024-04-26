#pragma once

#include <glm/glm.hpp>

#include <optional>

#include "Aliases.hpp"
#include "Texture.hpp"

namespace TotoEngine {

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};
static constexpr auto VERTEX_ATTRIBUTE_POSITION = 0;
static constexpr auto VERTEX_ATTRIBUTE_NORMAL = 1;
static constexpr auto VERTEX_ATTRIBUTE_UV = 2;
using Index = unsigned int;

struct PhongMaterial {
    ColorRGB ambient;
    ColorRGB diffuse;
    ColorRGB specular;
    ColorRGB emissive;
    float shininess;
    float opacity;
};
struct PBRMaterial {
    ColorRGB albedo;
    float metallic;
    float roughness;
    float ao;
    float opacity;
};

}

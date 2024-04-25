#pragma once

#include <glm/glm.hpp>
#include <optional>

namespace TotoEngine {

template<typename T>
using optional_ref = std::optional<std::reference_wrapper<T>>;

using Vector2 = glm::vec2;
using Vector3 = glm::vec3;
using Vector4 = glm::vec4;
using Matrix2 = glm::mat2;
using Matrix3 = glm::mat3;
using Matrix4 = glm::mat4;
using Quaternion = glm::quat;

using ColorRGB = Vector3;
using ColorRGBA = Vector4;

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

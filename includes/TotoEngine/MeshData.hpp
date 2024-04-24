#pragma once

#include <glm/glm.hpp>

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

struct Material {};

}

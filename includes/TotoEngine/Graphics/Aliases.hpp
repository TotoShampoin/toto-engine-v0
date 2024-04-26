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

}

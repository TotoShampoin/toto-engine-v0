#pragma once

#include <functional>
#include <glm/glm.hpp>
#include <optional>

namespace TotoEngine {

template<typename T>
using optional_ref = std::optional<std::reference_wrapper<T>>;

}

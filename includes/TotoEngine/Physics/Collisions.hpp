#pragma once

#include "TotoEngine/Core/Logger.hpp"
#include "TotoEngine/Math/Primitives.hpp"
#include "TotoEngine/Math/Transform.hpp"
#include <array>
#include <stdexcept>
#include <variant>
#include <vector>

namespace TotoEngine {

namespace Physics {

struct BoundingBox {
    Math::Vector3 size {1, 1, 1};

    std::array<Math::Vector3, 8> vertices() const {
        return {
            Math::Vector3(-.5, -.5, -.5) * size, //
            Math::Vector3(-.5, -.5, .5) * size,  //
            Math::Vector3(-.5, .5, -.5) * size,  //
            Math::Vector3(-.5, .5, .5) * size,   //
            Math::Vector3(.5, -.5, -.5) * size,  //
            Math::Vector3(.5, -.5, .5) * size,   //
            Math::Vector3(.5, .5, -.5) * size,   //
            Math::Vector3(.5, .5, .5) * size
        };
    }
};
struct BoundingSphere {
    float radius {1};
};

using Bounding = std::variant<BoundingBox, BoundingSphere>;

constexpr auto NO_TRANSFORM = Math::Transform();
struct BoundingBoxTransformed {
    const BoundingBox& bounds;
    const Math::Transform& transform = NO_TRANSFORM;
};
struct BoundingSphereTransformed {
    const BoundingSphere& bounds;
    const Math::Transform& transform = NO_TRANSFORM;
};

using BoundingTransformed = std::variant<BoundingBoxTransformed, BoundingSphereTransformed>;

class Collision {
public:
    Collision() = default;
    Collision& add(const Bounding& bounds, const Math::Transform& transform) {
        std::visit([&](auto&& bound) { add(bound, transform); }, bounds);
        return *this;
    }
    Collision& add(const BoundingBox& bounds, const Math::Transform& transform) {
        _colliders.emplace_back(BoundingBoxTransformed(bounds, transform));
        return *this;
    }
    Collision& add(const BoundingSphere& bounds, const Math::Transform& transform) {
        _colliders.emplace_back(BoundingSphereTransformed(bounds, transform));
        return *this;
    }

    bool collision() const {
        if (_colliders.size() < 2) {
            throw std::runtime_error("Colliders not set");
        }
        return std::visit(
            [](const auto& collider_1, const auto& collider_2) { return collides(collider_1, collider_2); },
            _colliders[0], _colliders[1]
        );
    }

private:
    std::vector<BoundingTransformed> _colliders;

    static bool collides(const BoundingBoxTransformed&, const BoundingBoxTransformed&);
    static bool collides(const BoundingSphereTransformed&, const BoundingSphereTransformed&);
    static bool collides(const BoundingBoxTransformed&, const BoundingSphereTransformed&);

    // symmetric function calls
    static bool collides(const BoundingSphereTransformed& sphere, const BoundingBoxTransformed& box) {
        return collides(box, sphere);
    }
};

inline void warnIfTransformScalingIsNotUniform(const Math::Transform& transform) {
    if (transform.scale().x != transform.scale().y || transform.scale().x != transform.scale().z) {
        Core::Logger::warn("Non-uniform bounding box size will not work as expected");
    }
}

} // namespace Physics

} // namespace TotoEngine

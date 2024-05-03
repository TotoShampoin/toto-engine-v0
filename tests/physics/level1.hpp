#pragma once
#include <TotoEngine/TotoEngine.hpp>

namespace Test_Physics {

using namespace TotoEngine;

namespace Level1 {

// Level 1: Hard coded coordinates
struct Sphere {
    float radius;
    Math::Vector3 center {};
};
struct AABB {
    Math::Vector3 position {};
    Math::Vector3 size {};

    Math::Vector3 min() const {
        return position - size / 2.f;
    }
    Math::Vector3 max() const {
        return position + size / 2.f;
    }
};
inline bool collides(const Sphere& a, const Sphere& b) {
    auto distance = glm::distance(a.center, b.center);
    return distance < a.radius + b.radius;
}
inline bool collides(const AABB& a, const AABB& b) {
    Math::Vector3 a_min = a.min();
    Math::Vector3 a_max = a.max();
    Math::Vector3 b_min = b.min();
    Math::Vector3 b_max = b.max();
    for(int i = 0; i < 3; i++) {
        if(a_max[i] < b_min[i] || a_min[i] > b_max[i]) {
            return false;
        }
    }
    return true;
}
inline bool collides(const Sphere& a, const AABB& b) {
    Math::Vector3 closest_point = b.position;
    for(int i = 0; i < 3; i++) {
        closest_point[i] = glm::clamp(a.center[i], b.min()[i], b.max()[i]);
    }
    return glm::distance(a.center, closest_point) < a.radius;
}
inline bool collides(const AABB& a, const Sphere& b) { return collides(b, a); }

}

}

#include "TotoEngine/Physics/Collisions.hpp"
#include "TotoEngine/Math/Primitives.hpp"
#include <algorithm>


namespace TotoEngine {

namespace Physics {

float getScaledRadius(float radius, Math::Vector3 scale) {
    return radius * std::max({ scale.x, scale.y, scale.z });
}

bool Collision::collides(const BoundingBoxTransformed& a, const BoundingBoxTransformed& b) {
    auto a_vertices = a.bounds.vertices();
    auto b_vertices = b.bounds.vertices();
    for(auto& vertex : a_vertices) {
        vertex = a.transform.matrix() * Math::Vector4(vertex, 1);
    }
    for(auto& vertex : b_vertices) {
        vertex = b.transform.matrix() * Math::Vector4(vertex, 1);
    }
    auto axes = {
        glm::normalize(a_vertices[1] - a_vertices[0]),
        glm::normalize(a_vertices[2] - a_vertices[0]),
        glm::normalize(a_vertices[4] - a_vertices[0]),
        glm::normalize(b_vertices[1] - b_vertices[0]),
        glm::normalize(b_vertices[2] - b_vertices[0]),
        glm::normalize(b_vertices[4] - b_vertices[0])
    };

    for(auto& axis : axes) {
        float a_min = std::numeric_limits<float>::infinity();
        float a_max = -std::numeric_limits<float>::infinity();
        float b_min = std::numeric_limits<float>::infinity();
        float b_max = -std::numeric_limits<float>::infinity();

        for(auto& vertex : a_vertices) {
            float projection = glm::dot(vertex, axis);
            a_min = std::min(a_min, projection);
            a_max = std::max(a_max, projection);
        }
        for(auto& vertex : b_vertices) {
            float projection = glm::dot(vertex, axis);
            b_min = std::min(b_min, projection);
            b_max = std::max(b_max, projection);
        }

        if(a_max < b_min || a_min > b_max) {
            return false;
        }
    }

    return true;
}
bool Collision::collides(const BoundingSphereTransformed& a, const BoundingSphereTransformed& b) {
    auto a_radius = getScaledRadius(a.bounds.radius, a.transform.scale());
    auto b_radius = getScaledRadius(b.bounds.radius, b.transform.scale());
    return glm::distance(a.transform.position(), b.transform.position()) < a_radius + b_radius;
}
bool Collision::collides(const BoundingBoxTransformed& a, const BoundingSphereTransformed& b) {
    auto local_to_box_sphere = Math::Transform::fromMatrix(a.transform.inverseMatrix() * b.transform.matrix());
    auto local_position = local_to_box_sphere.position();
    auto local_radius = getScaledRadius(b.bounds.radius, b.transform.scale());
    auto closest_point = glm::clamp(local_position, -a.bounds.size / 2.f, a.bounds.size / 2.f);
    return glm::distance(local_position, closest_point) < local_radius;
}

}

}

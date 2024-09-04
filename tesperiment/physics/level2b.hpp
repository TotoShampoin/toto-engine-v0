#pragma once
#include "TotoEngine/Math/Primitives.hpp"
#include "TotoEngine/Math/Transform.hpp"
#include <TotoEngine/TotoEngine.hpp>

#include <glm/geometric.hpp>
#include <glm/vector_relational.hpp>

namespace Test_Physics {

using namespace TotoEngine;

namespace Level2b {

inline void warnIfNotSquareScale(const Math::Transform& transform) {
    const auto& scale = transform.scale();
    if(scale.x != scale.y || scale.y != scale.z) {
        Core::Logger::warn("Non-uniform scaling on collisions will not work as expected");
    }
}

struct SphereCollision {
    float radius { 1 };

    bool collides(const Math::Vector3& point) const {
        return glm::distance({ 0, 0, 0 }, point) < radius;
    }

    float scaledRadius(const Math::Transform& transform) const {
        const auto& scale = transform.scale();
        float scale_factor = (scale.x + scale.y + scale.z) / 3;
        return radius * scale_factor;
    }
};
struct SphereCollisionTransformed {
    SphereCollision collision;
    Math::Transform transform {};

    float scaledRadius() const {
        return collision.scaledRadius(transform);
    }
};

struct BoxCollision {
    Math::Vector3 size { 1, 1, 1 };

    std::array<Math::Vector3, 8> vertices() const {
        return {
            Math::Vector3(-.5, -.5, -.5) * size,
            Math::Vector3(-.5, -.5,  .5) * size,
            Math::Vector3(-.5,  .5, -.5) * size,
            Math::Vector3(-.5,  .5,  .5) * size,
            Math::Vector3( .5, -.5, -.5) * size,
            Math::Vector3( .5, -.5,  .5) * size,
            Math::Vector3( .5,  .5, -.5) * size,
            Math::Vector3( .5,  .5,  .5) * size
        };
    }

    bool collides(const Math::Vector3& point) const {
        return glm::all(glm::lessThanEqual(glm::abs(point), size / 2.f));
    }
};
struct BoxCollisionTransformed {
    BoxCollision collision;
    Math::Transform transform {};
};


inline bool collides(const SphereCollisionTransformed& a, const SphereCollisionTransformed& b) {
    auto a_radius = a.scaledRadius();
    auto b_radius = b.scaledRadius();
    return glm::distance(a.transform.position(), b.transform.position()) < a_radius + b_radius;
}

inline bool collides(const BoxCollisionTransformed& a, const BoxCollisionTransformed& b) {
    auto a_vertices = a.collision.vertices();
    auto b_vertices = b.collision.vertices();
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

inline bool collides(const SphereCollisionTransformed& a, const BoxCollisionTransformed& b) {
    auto local_to_box_sphere = Math::Transform::fromMatrix(b.transform.inverseMatrix() * a.transform.matrix());
    auto local_position = local_to_box_sphere.position();
    auto local_radius = a.collision.scaledRadius(local_to_box_sphere);
    auto closest_point = glm::clamp(local_position, -b.collision.size / 2.f, b.collision.size / 2.f);
    return glm::distance(local_position, closest_point) < local_radius;
}

inline bool collides(const BoxCollisionTransformed& a, const SphereCollisionTransformed& b) {
    return collides(b, a);
}


inline void renderSphere(const Graphics::Camera& camera, const SphereCollisionTransformed& sphere, const Math::ColorRGB& color = {1, 1, 1}) {
    static auto material = Graphics::BasicMaterial();
    static auto mesh = Graphics::sphere(1, 32, 16);
    static auto transform = Math::Transform();

    material.color = color;
    transform = sphere.transform;
    transform.scale() *= sphere.collision.radius;

    Graphics::Renderer::bind(mesh, material.shader());
    Graphics::Renderer::apply(material.shader(), camera, transform);
    material.apply();
    Graphics::Renderer::draw(mesh);
}

inline void renderBox(const Graphics::Camera& camera, const BoxCollisionTransformed& box, const Math::ColorRGB& color = {1, 1, 1}) {
    static auto material = Graphics::BasicMaterial();
    static auto mesh = Graphics::cube(1, 1, 1);
    static auto transform = Math::Transform();

    material.color = color;
    transform = box.transform;
    transform.scale() *= box.collision.size;

    Graphics::Renderer::bind(mesh, material.shader());
    Graphics::Renderer::apply(material.shader(), camera, transform);
    material.apply();
    Graphics::Renderer::draw(mesh);
}

}

}

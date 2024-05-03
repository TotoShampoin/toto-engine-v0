#pragma once
#include <TotoEngine/TotoEngine.hpp>
#include <array>
#include <glm/geometric.hpp>
#include <glm/vector_relational.hpp>

#include "TotoEngine/Math/Primitives.hpp"
#include "TotoEngine/Math/Transform.hpp"

namespace Test_Physics {

using namespace TotoEngine;

// Level 2: Transformed hitboxes
// Full transforms not supported
// TODO(Physics): Find a way to use scaling on spheres
namespace Level2 {

class Sphere : private Math::Transform {
public:
    Sphere(const float& radius = 1.f) : _radius(radius) {}

    [[nodiscard]] float& radius() { return _radius; }
    [[nodiscard]] float radius() const { return _radius; }

    using Math::Transform::translate;
    using Math::Transform::matrix;
    using Math::Transform::position;
    using Math::Transform::translationMatrix;
private:
    float _radius;
};

class Box : public Math::Transform {
public:
    Box(const Math::Vector3& size = {1, 1, 1}) {
        scale() = size;
    }

    std::array<Math::Vector3, 8> vertices() const {
        return {
            matrix() * Math::Vector4(-.5, -.5, -.5, 1),
            matrix() * Math::Vector4(-.5, -.5,  .5, 1),
            matrix() * Math::Vector4(-.5,  .5, -.5, 1),
            matrix() * Math::Vector4(-.5,  .5,  .5, 1),
            matrix() * Math::Vector4( .5, -.5, -.5, 1),
            matrix() * Math::Vector4( .5, -.5,  .5, 1),
            matrix() * Math::Vector4( .5,  .5, -.5, 1),
            matrix() * Math::Vector4( .5,  .5,  .5, 1)
        };
    }

    [[nodiscard]] Math::Vector3& size() { return scale(); }
    [[nodiscard]] Math::Vector3 size() const { return scale(); }
private:
};

inline bool collides(const Sphere& a, const Math::Vector3& b) {
    return glm::distance(a.position(), b) < a.radius();
}
inline bool collides(const Box& a, const Math::Vector3& b) {
    auto local_point = Math::Vector3(a.inverseMatrix() * Math::Vector4(b, 1));
    return glm::all(glm::lessThanEqual(glm::abs(local_point), Math::Vector3(.5)));
}

inline bool collides(const Sphere& a, const Sphere& b) {
    auto distance = glm::distance(a.position(), b.position());
    return distance < a.radius() + b.radius();
}

// Separating Axis Theorem
inline bool collides(const Box& a, const Box& b) {
    auto a_vertices = a.vertices();
    auto b_vertices = b.vertices();

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

// Separating Axis Theorem again
inline bool collides(const Sphere& a, const Box& b) {
    auto b_vertices = b.vertices();

    auto axes = {
        glm::normalize(b_vertices[1] - b_vertices[0]),
        glm::normalize(b_vertices[2] - b_vertices[0]),
        glm::normalize(b_vertices[4] - b_vertices[0])
    };

    for (const auto& axis : axes) {
        float a_min = std::numeric_limits<float>::infinity();
        float a_max = -std::numeric_limits<float>::infinity();
        float b_min = std::numeric_limits<float>::infinity();
        float b_max = -std::numeric_limits<float>::infinity();

        for (int i = 0; i < 3; i++) {
            float projection = glm::dot(a.position(), axis);
            a_min = std::min(a_min, projection - a.radius());
            a_max = std::max(a_max, projection + a.radius());
        }
        for (auto& vertex : b_vertices) {
            float projection = glm::dot(vertex, axis);
            b_min = std::min(b_min, projection);
            b_max = std::max(b_max, projection);
        }

        if (a_max < b_min || a_min > b_max) {
            return false;
        }
    }

    return true;
}
inline bool collides(const Box& a, const Sphere& b) { return collides(b, a); }


inline void renderSphere(const Graphics::Camera& camera, const Sphere& sphere, const Math::ColorRGB& color = {1, 1, 1}) {
    static auto material = Graphics::BasicMaterial();
    static auto mesh = Graphics::sphere(1, 32, 16);
    static auto transform = Math::Transform();

    material.color = color;
    transform.position() = sphere.position();
    transform.scale() = Math::Vector3(sphere.radius());

    Graphics::Renderer::bind(mesh, material.shader());
    Graphics::Renderer::apply(material.shader(), camera, transform);
    material.apply();
    Graphics::Renderer::draw(mesh);
}

inline void renderBox(const Graphics::Camera& camera, const Box& box, const Math::ColorRGB& color = {1, 1, 1}) {
    static auto material = Graphics::BasicMaterial();
    static auto mesh = Graphics::cube(1, 1, 1);
    static auto transform = Math::Transform();

    material.color = color;
    transform.position() = box.position();
    transform.scale() = box.scale();
    transform.rotation() = box.rotation();

    Graphics::Renderer::bind(mesh, material.shader());
    Graphics::Renderer::apply(material.shader(), camera, transform);
    material.apply();
    Graphics::Renderer::draw(mesh);
}

}

}

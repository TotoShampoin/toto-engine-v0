#include "TotoEngine/Graphics/Shapes.hpp"
#include "TotoEngine/Graphics/GeometryBuffer.hpp"
#include <glm/gtc/constants.hpp>
#include <vector>

// TODO: Figure out UV for plane and cube

namespace TotoEngine {

constexpr auto PI = glm::pi<float>();
constexpr auto TAU = glm::tau<float>();

GeometryBuffer plane(float width, float height) {
    return GeometryBuffer(
        {
            {{ -width / 2.f, height / 2.f, 0 }, { 0, 0, 1 }, { 0, 0 }},
            {{ width / 2.f, height / 2.f, 0 }, { 0, 0, 1 }, { 1, 0 }},
            {{ width / 2.f, -height / 2.f, 0 }, { 0, 0, 1 }, { 1, 1 }},
            {{ -width / 2.f, -height / 2.f, 0 }, { 0, 0, 1 }, { 0, 1 }},
        }, {
            0, 1, 2,
            0, 2, 3,
        }
    );
}

GeometryBuffer cube(float width, float height, float depth) {
    return GeometryBuffer(
        {
            // front
            { { -width / 2.f, height / 2.f, depth / 2.f }, { 0, 0, 1 }, { 0, 0 } },
            { { width / 2.f, height / 2.f, depth / 2.f }, { 0, 0, 1 }, { 1, 0 } },
            { { width / 2.f, -height / 2.f, depth / 2.f }, { 0, 0, 1 }, { 1, 1 } },
            { { -width / 2.f, -height / 2.f, depth / 2.f }, { 0, 0, 1 }, { 0, 1 } },
            // back
            { { -width / 2.f, height / 2.f, -depth / 2.f }, { 0, 0, -1 }, { 1, 0 } },
            { { width / 2.f, height / 2.f, -depth / 2.f }, { 0, 0, -1 }, { 0, 0 } },
            { { width / 2.f, -height / 2.f, -depth / 2.f }, { 0, 0, -1 }, { 0, 1 } },
            { { -width / 2.f, -height / 2.f, -depth / 2.f }, { 0, 0, -1 }, { 1, 1 } },
            // left
            { { -width / 2.f, height / 2.f, -depth / 2.f }, { -1, 0, 0 }, { 0, 0 } },
            { { -width / 2.f, height / 2.f, depth / 2.f }, { -1, 0, 0 }, { 1, 0 } },
            { { -width / 2.f, -height / 2.f, depth / 2.f }, { -1, 0, 0 }, { 1, 1 } },
            { { -width / 2.f, -height / 2.f, -depth / 2.f }, { -1, 0, 0 }, { 0, 1 } },
            // right
            { { width / 2.f, height / 2.f, -depth / 2.f }, { 1, 0, 0 }, { 1, 0 } },
            { { width / 2.f, height / 2.f, depth / 2.f }, { 1, 0, 0 }, { 0, 0 } },
            { { width / 2.f, -height / 2.f, depth / 2.f }, { 1, 0, 0 }, { 0, 1 } },
            { { width / 2.f, -height / 2.f, -depth / 2.f }, { 1, 0, 0 }, { 1, 1 } },
            // top
            { { -width / 2.f, height / 2.f, -depth / 2.f }, { 0, 1, 0 }, { 0, 0 } },
            { { width / 2.f, height / 2.f, -depth / 2.f }, { 0, 1, 0 }, { 1, 0 } },
            { { width / 2.f, height / 2.f, depth / 2.f }, { 0, 1, 0 }, { 1, 1 } },
            { { -width / 2.f, height / 2.f, depth / 2.f }, { 0, 1, 0 }, { 0, 1 } },
            // bottom
            { { -width / 2.f, -height / 2.f, -depth / 2.f }, { 0, -1, 0 }, { 1, 0 } },
            { { width / 2.f, -height / 2.f, -depth / 2.f }, { 0, -1, 0 }, { 0, 0 } },
            { { width / 2.f, -height / 2.f, depth / 2.f }, { 0, -1, 0 }, { 0, 1 } },
            { { -width / 2.f, -height / 2.f, depth / 2.f }, { 0, -1, 0 }, { 1, 1 } },
        }, {
            2, 1, 0, 3, 2, 0, // front
            5, 6, 4, 6, 7, 4, // back
            10, 9, 8, 11, 10, 8, // left
            13, 14, 12, 14, 15, 12, // right
            18, 17, 16, 19, 18, 16, // top
            21, 22, 20, 22, 23, 20, // bottom
        }
    );
}

GeometryBuffer sphere(float radius, int slices, int stacks) {
    std::vector<Vertex> vertices;
    std::vector<Index> indices;
    for(int i = 0; i <= stacks; i++) {
        float v = i / (float)stacks;
        float phi = v * PI;
        for(int j = 0; j <= slices; j++) {
            float u = j / (float)slices;
            float theta = u * TAU;
            float x = glm::cos(theta) * glm::sin(phi);
            float y = glm::cos(phi);
            float z = glm::sin(theta) * glm::sin(phi);
            vertices.push_back({{x * radius, y * radius, z * radius}, {x, y, z}, {1-u, 1-v}});
        }
    }
    for(int i = 0; i < slices * stacks + slices; i++) {
        indices.push_back(i);
        indices.push_back(i + slices + 1);
        indices.push_back(i + slices);
        indices.push_back(i + slices + 1);
        indices.push_back(i);
        indices.push_back(i + 1);
    }
    return GeometryBuffer(vertices, indices);
}

// GeometryBuffer cylinder(float radius, float height, int slices, int stacks) {
//     //
// }

// GeometryBuffer cone(float radius, float height, int slices, int stacks) {
//     //
// }

// GeometryBuffer torus(float radius, float tube_radius, int slices, int stacks) {
//     //
// }

// GeometryBuffer capsule(float radius, float height, int slices, int stacks) {
//     //
// }

}
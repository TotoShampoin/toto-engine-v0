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
            {{-width / 2, -height / 2, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
            {{-width / 2, height / 2, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
            {{width / 2, height / 2, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
            {{width / 2, -height / 2, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
        }, {
            0, 1, 2,
            0, 2, 3,
        }
    );
}

GeometryBuffer cube(float width, float height, float depth) {
    return GeometryBuffer(
        {
            // Front
            {{width / 2, -height / 2, depth / 2}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
            {{width / 2, height / 2, depth / 2}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
            {{-width / 2, height / 2, depth / 2}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
            {{-width / 2, -height / 2, depth / 2}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
            // Back
            {{width / 2, -height / 2, -depth / 2}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
            {{-width / 2, -height / 2, -depth / 2}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
            {{-width / 2, height / 2, -depth / 2}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
            {{width / 2, height / 2, -depth / 2}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
            // Right
            {{width / 2, -height / 2, -depth / 2}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
            {{width / 2, height / 2, -depth / 2}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
            {{width / 2, height / 2, depth / 2}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
            {{width / 2, -height / 2, depth / 2}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            // Left
            {{-width / 2, -height / 2, -depth / 2}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            {{-width / 2, -height / 2, depth / 2}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
            {{-width / 2, height / 2, depth / 2}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
            {{-width / 2, height / 2, -depth / 2}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
            // Top
            {{width / 2, height / 2, -depth / 2}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
            {{-width / 2, height / 2, -depth / 2}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
            {{-width / 2, height / 2, depth / 2}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
            {{width / 2, height / 2, depth / 2}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
            // Bottom
            {{width / 2, -height / 2, depth / 2}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
            {{-width / 2, -height / 2, depth / 2}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
            {{-width / 2, -height / 2, -depth / 2}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
            {{width / 2, -height / 2, -depth / 2}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
        }, {
            0, 1, 2,  2, 3, 0,
            4, 5, 6,  6, 7, 4,
            8, 9, 10,  10, 11, 8,
            12, 13, 14,  14, 15, 12,
            16, 17, 18,  18, 19, 16,
            20, 21, 22,  22, 23, 20,
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
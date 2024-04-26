#pragma once

#include "GeometryBuffer.hpp"

namespace TotoEngine {

GeometryBuffer plane(float width = 1.0f, float height = 1.0f);

GeometryBuffer cube(float width = 1.0f, float height = 1.0f, float depth = 1.0f);

GeometryBuffer sphere(float radius = 1.0f, int slices = 16, int stacks = 8);

// GeometryBuffer cylinder(float radius = 1.0f, float height = 1.0f, int slices = 16, int stacks = 1);

// GeometryBuffer cone(float radius = 1.0f, float height = 1.0f, int slices = 16, int stacks = 1);

// GeometryBuffer torus(float radius = 1.0f, float tube_radius = 0.3f, int slices = 16, int stacks = 8);

// GeometryBuffer capsule(float radius = 1.0f, float height = 1.0f, int slices = 16, int stacks = 8);

}

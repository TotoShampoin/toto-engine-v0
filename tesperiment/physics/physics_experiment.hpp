#pragma once

#include <TotoEngine/TotoEngine.hpp>
#include <string>

#include "level2b.hpp"

namespace Test_Physics {
using namespace TotoEngine;

struct TestData {
    Graphics::Camera camera {glm::perspective(70.f, (float)800 / 600, .1f, 100.f)};
    // Graphics::Camera camera {glm::ortho(-20.f, 20.f, -15.f, 15.f, -100.f, 100.f)};
    Level2b::SphereCollisionTransformed hitbox_a {{2}};
    Level2b::SphereCollisionTransformed hitbox_b {{3}};
    Level2b::BoxCollisionTransformed hitbox_c {};
    Level2b::BoxCollisionTransformed hitbox_d {};

    float time {0};
};

inline void renderFloor_beta(const Graphics::Camera& camera);
void renderImgui_beta(Core::Window& window, TestData& data, const std::vector<std::string>& logs);
void physics_beta(TestData& data);
int test_physics_beta();

} // namespace Test_Physics
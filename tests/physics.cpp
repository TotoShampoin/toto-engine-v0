#include <TotoEngine/TotoEngine.hpp>

#include "TotoEngine/Math/Primitives.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Test_Physics {

using namespace TotoEngine;

// Level 1: Hard coded coordinates
// TODO(Physics): Level 2: Depend on Transforms for flexible collisions
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
bool collides(const Sphere& a, const Sphere& b) {
    auto distance = glm::distance(a.center, b.center);
    return distance < a.radius + b.radius;
}
bool collides(const AABB& a, const AABB& b) {
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
bool collides(const Sphere& a, const AABB& b) {
    Math::Vector3 closest_point = b.position;
    for(int i = 0; i < 3; i++) {
        closest_point[i] = glm::clamp(a.center[i], b.min()[i], b.max()[i]);
    }
    return glm::distance(a.center, closest_point) < a.radius;
}
bool collides(const AABB& a, const Sphere& b) { return collides(b, a); }

void renderSphere(const Graphics::Camera& camera, const Sphere& sphere, const Math::ColorRGB& color = {1, 1, 1}) {
    static auto material = Graphics::BasicMaterial();
    static auto mesh = Graphics::sphere(1, 32, 16);
    static auto transform = Math::Transform();

    transform.position() = sphere.center;
    transform.scale() = Math::Vector3(sphere.radius);
    material.color = color;

    Graphics::Renderer::bind(mesh, material.shader());
    Graphics::Renderer::apply(material.shader(), camera, transform);
    material.apply();
    Graphics::Renderer::draw(mesh);
}
void renderAABB(const Graphics::Camera& camera, const AABB& aabb, const Math::ColorRGB& color = {1, 1, 1}) {
    static auto material = Graphics::BasicMaterial();
    static auto mesh = Graphics::cube(1, 1, 1);
    static auto transform = Math::Transform();

    transform.position() = aabb.position;
    transform.scale() = aabb.size;
    material.color = color;

    Graphics::Renderer::bind(mesh, material.shader());
    Graphics::Renderer::apply(material.shader(), camera, transform);
    material.apply();
    Graphics::Renderer::draw(mesh);
}
void renderFloor(const Graphics::Camera& camera) {
    static auto mesh = Graphics::plane(25, 25);
    static auto material = Graphics::BasicMaterial();
    static auto transform = Math::Transform();
    
    transform.position() = {0, -10, 0};
    transform.lookAt({0, 0, 0});
    material.color = Math::ColorRGB(.25);

    Graphics::Renderer::bind(mesh, material.shader());
    material.apply();
    Graphics::Renderer::apply(material.shader(), camera, transform);
    Graphics::Renderer::draw(mesh);
}

struct TestData {
    Graphics::Camera camera {glm::perspective(70.f, (float)800/600, .1f, 100.f)};
    Sphere hitbox_a {2};
    Sphere hitbox_b {3};
    AABB hitbox_c {};
    AABB hitbox_d {};

    float time {0};
};

void renderImgui(Core::Window& /* window */, TestData& data) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always, ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(0, 0));
    ImGui::Begin("Physics Test", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings);
    ImGui::Text("Spheres");
    ImGui::Text("  Colliding: %s", collides(data.hitbox_a, data.hitbox_b) ? "true" : "false");
    ImGui::Text("AABBs");
    ImGui::Text("  Colliding: %s", collides(data.hitbox_c, data.hitbox_d) ? "true" : "false");
    ImGui::Text("Sphere-AABB");
    ImGui::Text("  Colliding: %s", collides(data.hitbox_b, data.hitbox_d) ? "true" : "false");
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void render(Core::Window& window, TestData& data) {
    Graphics::Renderer::clear();

    renderFloor(data.camera);
    renderSphere(data.camera, data.hitbox_a, {1, 0, 0});
    renderSphere(data.camera, data.hitbox_b, {0, 0, 1});
    renderAABB(data.camera, data.hitbox_c, {0, 1, 0});
    renderAABB(data.camera, data.hitbox_d, {1, 1, 0});

    renderImgui(window, data);
    Core::Window::swapBuffers(window);
}

void physics(TestData& data) {
    auto rotated_pos_1 = glm::cos(glm::radians(data.time * 10));
    auto rotated_pos_2 = glm::sin(glm::radians(data.time * 10));
    data.camera.position() = {5, -2.5, 20};
    data.camera.lookAt({0, 0, 0});

    data.hitbox_a.center = Math::Vector3(rotated_pos_1, 0, -rotated_pos_1) * 10.f;
    data.hitbox_d.position = Math::Vector3(3, rotated_pos_2 * 5, 3);
}

int test_physics() {
    auto window = Core::Window(800, 600, "Doing Physics");
    Graphics::Renderer::init();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window.GLFWWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460");

    auto data = TestData();

    data.hitbox_a.center = {5, 0, 0};
    data.hitbox_b.center = {0, 0, 0};
    data.hitbox_c.position = {5, -1, 5};
    data.hitbox_c.size = {2, 2, 2};
    data.hitbox_d.position = {5, 3, 5};
    data.hitbox_d.size = {4, 3, 4};

    Graphics::Renderer::enable(GL_DEPTH_TEST);
    while(!window.shouldClose()) {
        physics(data);
        render(window, data);
        Core::Window::pollEvents();
        data.time += 1/60.F;
    }

    return 0;
}

}

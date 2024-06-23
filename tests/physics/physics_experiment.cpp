#include <TotoEngine/TotoEngine.hpp>
#include <format>
#include <string>
#include "TotoEngine/Core/Logger.hpp"
#include "level2b.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Test_Physics {

using namespace TotoEngine;

inline void renderFloor_beta(const Graphics::Camera& camera) {
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
    // Graphics::Camera camera {glm::ortho(-20.f, 20.f, -15.f, 15.f, -100.f, 100.f)};
    Level2b::SphereCollisionTransformed hitbox_a {{2}};
    Level2b::SphereCollisionTransformed hitbox_b {{3}};
    Level2b::BoxCollisionTransformed hitbox_c {};
    Level2b::BoxCollisionTransformed hitbox_d {};

    float time {0};
};

void renderImgui_beta(Core::Window& window, TestData& data, const std::vector<std::string>& logs) {
    auto [width, height] = window.size();

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

    std::string full_logs;
    for(auto& log : logs)
        full_logs += log + "\n";
    ImGui::SetNextWindowPos(ImVec2(width, 0), ImGuiCond_Always, ImVec2(1, 0));
    ImGui::SetNextWindowSize(ImVec2(0, 0));
    ImGui::Begin("Logs", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings);
    ImGui::Text("%s", full_logs.c_str());
    ImGui::SetScrollHereY(1.0f);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void render_beta(Core::Window& window, TestData& data, const std::vector<std::string>& logs) {
    Graphics::Renderer::clear();

    renderFloor_beta(data.camera);
    Level2b::renderSphere(data.camera, data.hitbox_a, {1, 0, 0});
    Level2b::renderSphere(data.camera, data.hitbox_b, collides(data.hitbox_b, data.hitbox_d) ? Math::Vector3(1, 0, 1) : Math::Vector3(0, 0, 1));
    Level2b::renderBox(data.camera, data.hitbox_c, collides(data.hitbox_c, data.hitbox_d) ? Math::Vector3(0, 1, 1) : Math::Vector3(0, 1, 0));
    Level2b::renderBox(data.camera, data.hitbox_d, {1, 1, 0});

    renderImgui_beta(window, data, logs);
    Core::Window::swapBuffers(window);
}

void physics_beta(TestData& data) {
    auto rotated_pos_1 = glm::cos(glm::radians(data.time * 10));
    auto rotated_pos_2 = glm::sin(glm::radians(data.time * 10));
    auto rotated_pos_3 = glm::sin(glm::radians(data.time * 100));
    data.camera.position() = {10, 5, 20};
    data.camera.lookAt({0, 0, 0});

    data.hitbox_a.transform.position() = Math::Vector3(rotated_pos_1, 0, -rotated_pos_1) * 10.f;
    data.hitbox_d.transform.position() = Math::Vector3(3, rotated_pos_2 * 5, 2.5);
    data.hitbox_d.transform.rotation() = {0, 0, data.time / 2};
    data.hitbox_d.transform.scale() = {1, .75 + rotated_pos_3 * .25, 1};

    Level2b::warnIfNotSquareScale(data.hitbox_d.transform);
}

int test_physics_beta() {
    auto window = Core::Window(800, 600, "Doing Physics");
    Graphics::Renderer::init();
    Core::Logger::init();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window.GLFWWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460");

    auto logs = std::vector<std::string>();

    Core::Logger::setLogger([&logs](const char* message) {
        logs.push_back(std::format("[I] {}", message));
        if(logs.size() > 10)
            logs.erase(logs.begin());
    });
    Core::Logger::setWarningLogger([&logs](const char* message) {
        logs.push_back(std::format("[W] {}", message));
        if(logs.size() > 10)
            logs.erase(logs.begin());
    });
    Core::Logger::setErrorLogger([&logs](const char* message) {
        logs.push_back(std::format("[E] {}", message));
        if(logs.size() > 10)
            logs.erase(logs.begin());
    });

    auto data = TestData();

    data.hitbox_a.transform.position() = {5, 0, 0};
    data.hitbox_b.transform.position() = {0, 0, 0};
    data.hitbox_c.transform.position() = {5, -1, 5};
    data.hitbox_c.collision.size = {2, 2, 2};
    data.hitbox_d.transform.position() = {3, 2, 3};
    data.hitbox_d.collision.size = {5, 3, 5};

    Graphics::Renderer::enable(GL_DEPTH_TEST);
    while(!window.shouldClose()) {
        physics_beta(data);
        render_beta(window, data, logs);
        Core::Window::pollEvents();
        data.time += 1/60.F;
    }

    return 0;
}

}

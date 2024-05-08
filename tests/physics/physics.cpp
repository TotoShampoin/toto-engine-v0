#include "TotoEngine/Graphics/GPUObjects/GeometryBuffer.hpp"
#include "TotoEngine/Graphics/GPUObjects/ShaderProgram.hpp"
#include "TotoEngine/Graphics/RenderData/Light.hpp"
#include "TotoEngine/Graphics/Renderer.hpp"
#include "TotoEngine/Physics/Collisions.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <TotoEngine/TotoEngine.hpp>
#include <chrono>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/trigonometric.hpp>

namespace Test_Physics {

using namespace TotoEngine;

struct Object {
    Graphics::GeometryBufferInstance geometry;
    Graphics::PhongMaterial material;
    Math::Transform transform;
    Physics::Bounding bounds;
};
Object cube() {
    auto geometry = Graphics::GeometryBufferManager::create(
        Graphics::cube()
    );
    return { geometry, Graphics::PhongMaterial(), Math::Transform(), Physics::BoundingBox() };
}
Object sphere() {
    auto geometry = Graphics::GeometryBufferManager::create(
        Graphics::sphere()
    );
    return { geometry, Graphics::PhongMaterial(), Math::Transform(), Physics::BoundingSphere() };
}

int test_physics() {
    auto window = Core::Window(800, 600, "Doing Physics");
    Graphics::Renderer::init();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window.GLFWWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460");

    Core::Logger::init();
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

    auto camera = Graphics::Camera(glm::perspective(
        glm::radians(45.0f),
        800.0f / 600.0f,
        0.1f,
        100.0f
    ));

    camera.position() = { 0, 0, 5 };
    camera.lookAt({ 0, 0, 0 });

    auto amblight = Graphics::Light(Graphics::LightType::AMBIENT);
    auto dirlight = Graphics::Light(Graphics::LightType::DIRECTIONAL);

    amblight.intensity() = .25;
    dirlight.lookAt({1, 1, 1});

    auto cube1 = cube();
    auto cube2 = sphere();

    auto start = std::chrono::steady_clock::now();
    auto last = start;

    Graphics::Renderer::enable(GL_DEPTH_TEST);
    while(!window.shouldClose()) {
        auto now = std::chrono::steady_clock::now();
        auto time = std::chrono::duration<float>(now - start).count();
        auto delta = std::chrono::duration<float>(now - last).count();

        cube1.transform.position() = { 0, 0, 0 };
        cube1.transform.rotation() += delta;
        cube2.transform.position() = { 1.1, 0, 0 };
        cube2.transform.scale() = {.5, .5, .5};
        cube1.material.diffuse = { 1, 0, 0 };
        cube2.material.diffuse = { 0, 1, 0 };
        cube1.material.specular = {.5,.5,.5};
        cube2.material.specular = {.5,.5,.5};
        cube1.material.ambient = cube1.material.diffuse;
        cube2.material.ambient = cube2.material.diffuse;

        auto [width, height] = window.size();
        glViewport(0, 0, width, height);
        camera.projectionMatrix() = glm::perspective(
            glm::radians(45.0f),
            (float) width / height,
            0.1f,
            100.0f
        );
        Graphics::Renderer::clear();
        auto& shader = Graphics::PhongMaterial::shader();
        Graphics::ShaderProgram::use(shader);
        Graphics::Renderer::apply(shader, camera, {amblight, dirlight});
        cube1.material.apply();
        Graphics::Renderer::apply(shader, camera, cube1.transform);
        Graphics::GeometryBuffer::bind(cube1.geometry);
        Graphics::Renderer::draw(cube1.geometry);
        cube2.material.apply();
        Graphics::Renderer::apply(shader, camera, cube2.transform);
        Graphics::GeometryBuffer::bind(cube2.geometry);
        Graphics::Renderer::draw(cube2.geometry);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos({0, 0}, ImGuiCond_Always, { 0, 0 });
        ImGui::Begin("Time", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings);
        ImGui::Text("%f", time);
        ImGui::End();

        auto collision = Physics::Collision()
            .add(cube1.bounds, cube1.transform)
            .add(cube2.bounds, cube2.transform);

        auto cube2_on_screen = camera.worldToScreen(cube2.transform.position() + Math::Vector3(0, .75, 0));
        ImGui::SetNextWindowPos({ cube2_on_screen.x * width, cube2_on_screen.y * height }, ImGuiCond_Always, { 0.5f, 0.5f });
        ImGui::Begin("Cube 2 info", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings);
        ImGui::Text(collision.collision() ? "True" : "False");
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        Core::Window::swapBuffers(window);
        Core::Window::pollEvents();
        last = now;
    }

    return 0;
}

}

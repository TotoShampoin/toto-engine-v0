#include <TotoEngine/TotoEngine.hpp>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

#include "TotoEngine/Graphics/GPUObjects/GeometryBuffer.hpp"
#include "TotoEngine/Graphics/Renderer.hpp"
#include "TotoEngine/Math/Primitives.hpp"
#include "data.hpp"

int main() {
    using namespace TotoEngine;

    auto window = Core::Window(WIDTH, HEIGHT, "TotoEngine: Physically Based Rendering");
    Graphics::Renderer::init();

    Graphics::Renderer::enable(GL_DEPTH_TEST);

    auto camera = Graphics::Camera(glm::perspective(glm::radians(FOV), ASPECT, NEAR, FAR));

    auto screen_mesh = Graphics::GeometryBuffer(Graphics::plane(2, 2));
    auto screen_material = Graphics::ShaderProgram(
        Graphics::loadShaderFile<Graphics::ShaderType::VERTEX>("assets/screen.vert"),
        Graphics::loadShaderFile<Graphics::ShaderType::FRAGMENT>("assets/hdri.frag")
    );

    auto sphere_mesh = Graphics::GeometryBuffer(Graphics::sphere(1, 32, 16));
    auto sphere_material = Graphics::ShaderProgram(
        Graphics::loadShaderFile<Graphics::ShaderType::VERTEX>("assets/pbr.vert"),
        Graphics::loadShaderFile<Graphics::ShaderType::FRAGMENT>("assets/pbr.frag")
    );
    auto sphere_transform = Math::Transform();

    auto dir_light = Graphics::Light(Graphics::LightType::DIRECTIONAL, {1, 1, 1}, 1);
    dir_light.lookAt({1, 1, 1});

    auto hdri = Graphics::loadTexture2D("assets/hdri.jpg");
    auto hdri_diffuse = Graphics::loadTexture2D("assets/hdri-diffuse-RGBM.png");

    Graphics::Texture2D::bindAs(hdri, 0);
    Graphics::Texture2D::bindAs(hdri_diffuse, 1);

    Graphics::ShaderProgram::use(screen_material);
    screen_material.uniform("u_hdri", 0);

    Graphics::ShaderProgram::use(sphere_material);
    sphere_material.uniform("u_environment", 0);
    sphere_material.uniform("u_irradiance", 1);

    Math::ColorRGB albedo = {1, 1, 1};
    float roughness = 0.5f;
    float metallic = 0.5f;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window.GLFWWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460");

    auto init_time = std::chrono::high_resolution_clock::now();
    auto last_time = init_time;
    float shown_render_freq = 0;
    double cumul_render_time = 0;
    int cumul_frames = 0;

    float timer = 0;

    while (!window.shouldClose()) {
        auto time_at_start = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration<float>(time_at_start - init_time).count();
        auto delta_time = std::chrono::duration<float>(time_at_start - last_time).count();

        auto [width, height] = window.size();

        /* Rendering */ {
            Graphics::Renderer::clear();

            Graphics::Renderer::bind(screen_mesh, screen_material);
            Graphics::Renderer::apply(screen_material, camera);
            Graphics::Renderer::draw(screen_mesh);

            Graphics::Renderer::clear(false);

            Graphics::Renderer::bind(sphere_mesh, sphere_material);
            Graphics::Renderer::apply(sphere_material, camera, sphere_transform);
            Graphics::Renderer::apply(sphere_material, camera, {dir_light});

            sphere_material.uniform("u_albedo", albedo);
            sphere_material.uniform("u_metallic", metallic);
            sphere_material.uniform("u_roughness", roughness);

            Graphics::Renderer::draw(sphere_mesh);
        }
        auto time_after_render = std::chrono::high_resolution_clock::now();
        auto render_time = std::chrono::duration<float>(time_after_render - time_at_start).count();
        cumul_render_time += render_time;

        /* HUD */ {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::SetNextWindowPos(ImVec2(width, 0), ImGuiCond_Always, ImVec2(1, 0));
            ImGui::SetNextWindowSize(ImVec2(0, 0));
            ImGui::Begin(
                "##FPS", nullptr,
                ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
                    ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                    ImGuiWindowFlags_NoTitleBar
            );
            ImGui::Text("Render freq: %.0f", shown_render_freq);
            ImGui::End();

            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2(0, 0));
            ImGui::Begin(
                "##PARAMS", nullptr,
                ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
                    ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                    ImGuiWindowFlags_NoTitleBar
            );
            ImGui::ColorEdit3("Albedo", &albedo.r);
            ImGui::SliderFloat("Roughness", &roughness, 0, 1);
            ImGui::SliderFloat("Metallic", &metallic, 0, 1);
            ImGui::End();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        /* Update logic */ {
            float angle = 0.5f * time;
            camera.position() = {4 * glm::cos(angle), 1, 4 * glm::sin(angle)};
            camera.lookAt({0, 0, 0});
        }

        /* Time logic */ {
            last_time = time_at_start;
            timer += delta_time;
            cumul_frames++;
            if (timer > (float)1 / 5) {
                shown_render_freq = cumul_frames / cumul_render_time;
                cumul_frames = 0;
                cumul_render_time = 0;
                timer = 0;
            }
        }

        Core::Window::swapBuffers(window);
        Core::Window::pollEvents();
    }

    return 0;
}

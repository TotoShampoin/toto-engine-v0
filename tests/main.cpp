#include <TotoEngine/TotoEngine.hpp>

#include <algorithm>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <vector>

#include "TotoEngine/Graphics/ShaderProgram.hpp"
#include "TotoEngine/Graphics/Texture.hpp"
#include "TotoEngine/Graphics/TextureLoader.hpp"
#include "TotoEngine/Transform.hpp"

void imguiInit(TotoEngine::Window& window);
void imguiRender(float render_time);

using GLTexture = TotoEngine::GLObject<
    [] { GLuint id; glGenTextures(1, &id); return id; },
    [](GLuint& id) { glDeleteTextures(1, &id); }
>;

int main(int /* argc */, const char* /* argv */[]) {
    using namespace TotoEngine;
    using TotoEngine::TextureTarget::TEXTURE_2D;
    auto window = Window(800, 600, "TotoEngine");
    glewInit();

    imguiInit(window);

    auto camera_projection = glm::perspective(glm::radians(70.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    auto camera_transform = Transform();

    auto vertex_buffer = GeometryBuffer(
        {
            {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
            {{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
            {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
            {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}
        }, {
            0, 1, 2,
            2, 3, 0
        }
    );
    auto material = BasicMaterial();
    auto transform = Transform();

    material.map = loadTexture2D("tests_assets/smile.png");

    transform.translate({0.0f, 0.0f, -2.0f});

    auto start_time = std::chrono::high_resolution_clock::now();
    auto last_time = start_time;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    while(!window.shouldClose()) {
        auto current_time = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration<float>(current_time - start_time).count();
        // auto delta_time = std::chrono::duration<float>(current_time - last_time).count();

        Window::makeContextCurrent(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GeometryBuffer::bind(vertex_buffer);
        ShaderProgram::use(material.shader());
        material.shader().uniform("u_projection", camera_projection);
        material.shader().uniform("u_view", glm::inverse(camera_transform.matrix()));
        material.shader().uniform("u_model", transform.matrix());
        material.apply();
        glDrawElements(GL_TRIANGLES, vertex_buffer.indices().size(), GL_UNSIGNED_INT, nullptr);

        auto time_after_render = std::chrono::high_resolution_clock::now();
        auto render_time = std::chrono::duration<float>(time_after_render - current_time).count();

        imguiRender(render_time);

        Window::swapBuffers(window);
        Window::pollEvents();

        transform.rotate(glm::radians(1.0f), {0.0f, 1.0f, 0.0f});
        camera_transform.position().x = glm::cos(time);

        last_time = current_time;
    }

    return 0;
}


void imguiInit(TotoEngine::Window& window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window.GLFWWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460");
}
void imguiRender(float render_time) {
    static std::vector<float> render_times;
    render_times.push_back(1.f / render_time);
    if(render_times.size() > 200) {
        render_times.erase(render_times.begin());
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(0, 0));
    ImGui::Begin("Timing", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus);
    ImGui::PlotHistogram("##fps", render_times.data(), render_times.size(), 0, nullptr, 0, std::max_element(render_times.begin(), render_times.end()).operator*(), ImVec2(400, 80));
    ImGui::Text("Render FPS: %.2f", 1.f / render_time);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
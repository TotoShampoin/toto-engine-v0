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
#include "TotoEngine/Transform.hpp"

void imguiInit(TotoEngine::Window& window);
void imguiRender(float render_time, float delta_time);

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

    // TODO: Add a way to instantiate
    material.map = Texture2D(); {
        Texture2D::bind(material.map.value());
        Texture2D::parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        Texture2D::parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        Texture2D::parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        Texture2D::parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        constexpr int width = 16, height = 16, channels = 4;
        std::vector<uint8_t> data(width * height * channels, 0);
        for(int i = 0; i < width * height; i++) {
            data[i * channels + 0] = (i % 2) * 255;
            data[i * channels + 1] = (i % 3) * 127;
            data[i * channels + 2] = (i % 4) * 85;
            data[i * channels + 3] = 255;
        }
        Texture2D::image(width, height, channels, data);
        Texture2D::bind(Texture2D::NONE);
    }

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
        auto delta_time = std::chrono::duration<float>(current_time - last_time).count();

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

        imguiRender(render_time, delta_time);

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
void imguiRender(float render_time, float delta_time) {
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
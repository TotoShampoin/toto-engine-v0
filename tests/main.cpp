#include <TotoEngine/TotoEngine.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <vector>

#include "TotoEngine/Graphics/ShaderProgram.hpp"

void imguiInit(TotoEngine::Window& window);
void imguiRender();

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

    Texture2D texture;
    Texture2D::bind(texture);
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

    auto projection = glm::perspective(glm::radians(70.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    auto view = Matrix4(1.0f);
    auto model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));

    ShaderProgram::use(material.shader());
    material.shader().uniform("u_projection", projection);
    material.shader().uniform("u_view", view);
    material.shader().uniform("u_model", model);
    material.map = std::move(texture);
    material.apply();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    while(!window.shouldClose()) {
        Window::makeContextCurrent(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ShaderProgram::use(program);
        ShaderProgram::use(material.shader());
        GeometryBuffer::bind(vertex_buffer);
        glDrawElements(GL_TRIANGLES, vertex_buffer.indices().size(), GL_UNSIGNED_INT, nullptr);

        imguiRender();

        Window::swapBuffers(window);
        Window::pollEvents();

        model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        // program.uniform("u_model", model);
        material.shader().uniform("u_model", model);
    }

    return 0;
}


void imguiInit(TotoEngine::Window& window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window.GLFWWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460");
}
void imguiRender() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(0, 0));
    ImGui::Begin("FPS", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    ImGui::Text("%3.2f", ImGui::GetIO().Framerate);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
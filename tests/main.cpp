#include <GL/glew.h>
#include <TotoEngine/TotoEngine.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <iostream>
#include <vector>

#include "TotoEngine/Uniforms.hpp"
#include "res/shaders/vertex.glsl.hpp"
#include "res/shaders/fragment.glsl.hpp"


int main(int /* argc */, const char* /* argv */[]) {
    using namespace TotoEngine;
    Window window(800, 600, "TotoEngine");
    glewInit();

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
    auto program = ShaderProgram(
        VertexShaderFile(vertex), 
        FragmentShaderFile(fragment)
    );

    auto projection = glm::perspective(glm::radians(70.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    auto view = Matrix4(1.0f);
    auto model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));

    ShaderProgram::use(program);
    program.uniform("u_projection", projection);
    program.uniform("u_view", view);
    program.uniform("u_model", model);
    try { // voluntary error
        program.uniform("u_inexistent_uniform", 0);
    } catch(const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window.GLFWWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_NONE);
    while(!window.shouldClose()) {
        Window::makeContextCurrent(window);

        {// OpenGL rendering
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            GeometryBuffer::bind(vertex_buffer);
            ShaderProgram::use(program);
            glDrawElements(GL_TRIANGLES, vertex_buffer.indices().size(), GL_UNSIGNED_INT, nullptr);
        }

        {// ImGui rendering
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

        Window::swapBuffers(window);
        Window::pollEvents();

        model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        program.uniform("u_model", model);
    }

    return 0;
}

#include <GL/glew.h>
#include <TotoEngine/TotoEngine.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <vector>

#include "res/shaders/vertex.glsl.hpp"
#include "res/shaders/fragment.glsl.hpp"


int main(int /* argc */, const char* /* argv */[]) {
    TotoEngine::printInfo();
    TotoEngine::Window::init();

    TotoEngine::Window window(800, 600, "TotoEngine");
    window.makeContextCurrent();

    glewInit();
    
    window.makeContextCurrent();

    auto vertex_buffer = TotoEngine::VertexBuffer(
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
    auto program = TotoEngine::ShaderProgram(
        TotoEngine::VertexShaderFile(vertex), 
        TotoEngine::FragmentShaderFile(fragment)
    );

    auto projection = glm::perspective(glm::radians(70.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    auto view = glm::mat4(1.0f);
    auto model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));

    TotoEngine::ShaderProgram::use(program);
    auto u_projection = glGetUniformLocation(program.program(), "u_projection");
    auto u_view = glGetUniformLocation(program.program(), "u_view");
    auto u_model = glGetUniformLocation(program.program(), "u_model");

    glUniformMatrix4fv(u_projection, 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(u_view, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(u_model, 1, GL_FALSE, &model[0][0]);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window.GLFWWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_NONE);
    while(!window.shouldClose()) {
        window.makeContextCurrent();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        TotoEngine::VertexBuffer::bind(vertex_buffer);
        TotoEngine::ShaderProgram::use(program);
        glDrawElements(GL_TRIANGLES, vertex_buffer.indices().size(), GL_UNSIGNED_INT, nullptr);

        model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(u_model, 1, GL_FALSE, &model[0][0]);

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
        
        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}

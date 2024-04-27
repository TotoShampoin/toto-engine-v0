#include "TotoEngine/Graphics/GeometryBuffer.hpp"
#include "TotoEngine/Graphics/Renderer.hpp"
#include "TotoEngine/Graphics/ShaderFile.hpp"
#include "TotoEngine/Graphics/ShaderProgram.hpp"
#include "TotoEngine/Graphics/Shapes.hpp"
#include "TotoEngine/Graphics/Texture.hpp"
#include <GL/gl.h>
#include <TotoEngine/TotoEngine.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glm/trigonometric.hpp>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <vector>
#include <algorithm>
#include <chrono>

void imguiInit(TotoEngine::Window& window);

using GLTexture = TotoEngine::GLObject<
    [] { GLuint id; glGenTextures(1, &id); return id; },
    [](GLuint& id) { glDeleteTextures(1, &id); }
>;

int main(int /* argc */, const char* /* argv */[]) {
    using namespace TotoEngine;
    using TotoEngine::TextureTarget::TEXTURE_2D;
    auto window = Window(800, 600, "TotoEngine");
    glewInit();

    auto frame_buffer = FrameBuffer(320, 240);
    Texture2D::bind(frame_buffer.texture());
    Texture2D::parameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    Texture2D::parameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    Texture2D::parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    Texture2D::parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    imguiInit(window);
    auto [width, height] = window.size();

    auto camera = Camera(glm::perspective(glm::radians(70.0f), 320.0f / 240.0f, 0.1f, 100.0f));

    auto hdri_texture = Texture2DManager::create(loadTexture2D("tests_assets/hdri.jpg"));
    auto hdri_model = GeometryBuffer(
        {
            {{-width / 2, -height / 2, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
            {{width / 2, -height / 2, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
            {{width / 2, height / 2, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
            {{-width / 2, height / 2, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
        }, {
            0, 1, 2,
            0, 2, 3,
        }
    
    );
    auto hdri_shader = ShaderProgram(
        VertexShaderFile(std::ifstream("tests_assets/hdri.vert")),
        FragmentShaderFile(std::ifstream("tests_assets/hdri.frag"))
    );
    auto screen_shader = ShaderProgram(
        VertexShaderFile(std::ifstream("tests_assets/screen.vert")),
        FragmentShaderFile(std::ifstream("tests_assets/screen.frag"))
    );

    auto material = PhongMaterial();

    auto plane_model = cube(2, 2, 2);
    auto plane_transform = Transform();
    plane_transform.position() = {0, -2, -5};
    // plane_transform.lookAt({0, -1, -4});

    auto sphere_model = sphere(1, 32, 16);
    auto sphere_transform = Transform();

    auto amb_light = Light(LightType::AMBIENT, ColorRGB(1.0f, 1.0f, 1.0f), 0.33333f);
    auto dir_light = Light(LightType::DIRECTIONAL, ColorRGB(1.0f, 1.0f, 1.0f), 1.0f);
    dir_light.position() = {0, 0, 0};
    auto pt_light = Light(LightType::POINT, ColorRGB(1.0f, 1.0f, 1.0f), 2.0f);
    pt_light.position() = {0, 2, -4};

    glm::vec3 dir_light_target = {0, 1, 1};

    auto light_helper_transform = Transform();
    light_helper_transform.scale() *= .1;

    material.diffuse_map = Texture2DManager::create(loadTexture2D("tests_assets/uv.png"));
    material.specular = ColorRGB(.5f);
    material.shininess = 64.f;
    material.ambient_map = Texture2DManager::create(loadTexture2D("tests_assets/uv.png"));

    sphere_transform.translate({0.0f, 0.0f, -5.0f});

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
        
        Renderer::bindRenderTarget(frame_buffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GeometryBuffer::bind(hdri_model);
        ShaderProgram::use(hdri_shader);
        glActiveTexture(GL_TEXTURE0);
        Texture2D::bind(hdri_texture);
        hdri_shader.uniform("u_map", 0);
        hdri_shader.uniform("u_view", camera.viewMatrix());
        hdri_shader.uniform("u_projection", camera.projectionMatrix());
        Renderer::draw(hdri_model);

        glClear(GL_DEPTH_BUFFER_BIT);

        GeometryBuffer::bind(sphere_model);
        ShaderProgram::use(material.shader());
        material.apply();
        Renderer::apply(material.shader(), {dir_light, pt_light}, camera);
        Renderer::apply(material.shader(), sphere_transform, camera);

        Renderer::draw(sphere_model);

        GeometryBuffer::bind(plane_model);
        Renderer::apply(material.shader(), plane_transform, camera);
        Renderer::draw(plane_model);

        Renderer::bindRenderTarget(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GeometryBuffer::bind(hdri_model);
        ShaderProgram::use(screen_shader);
        glActiveTexture(GL_TEXTURE0);
        Texture2D::bind(frame_buffer.texture());
        screen_shader.uniform("u_map", 0);
        Renderer::draw(hdri_model);

        auto time_after_render = std::chrono::high_resolution_clock::now();
        auto render_time = std::chrono::duration<float>(time_after_render - current_time).count();


        {// imgui
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

            ImGui::SetNextWindowPos(ImVec2(width, 0), ImGuiCond_Always, ImVec2(1, 0));
            ImGui::SetNextWindowSize(ImVec2(0, 0));
            ImGui::Begin("Light", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus);
            ImGui::SliderFloat3("Dir light Target", &dir_light_target.x, -2, 2);
            ImGui::SliderFloat3("Pt light Position", &pt_light.position().x, -10, 10);
            ImGui::SliderFloat3("Camera Position", &camera.position().x, -10, 10);
            ImGui::End();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        Window::swapBuffers(window);
        Window::pollEvents();

        plane_transform.rotation().x = time;
        // transform.rotate(glm::radians(1.0f), {0.0f, 1.0f, 0.0f});
        sphere_transform.position().x = glm::sin(time);
        sphere_transform.rotation().y = time;
        // sphere_transform.rotation() += Vector3(2,3,5) * glm::radians(.1f);
        // camera.position().x = glm::cos(time);
        // camera.position().y = glm::sin(time) + 5;
        camera.lookAt({0, 0, -5});
        dir_light.lookAt(dir_light_target);
        light_helper_transform.position() = pt_light.position();

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

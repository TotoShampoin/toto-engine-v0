#include "TotoEngine/Graphics/FrameBuffer.hpp"
#include "TotoEngine/Graphics/Texture.hpp"
#include <TotoEngine/TotoEngine.hpp>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

#include <vector>
#include <algorithm>
#include <chrono>

void imguiInit(TotoEngine::Window& window);

int main(int /* argc */, const char* /* argv */[]) {
    using namespace TotoEngine;
    using TotoEngine::TextureTarget::TEXTURE_2D;
    auto window = Window(800, 600, "TotoEngine");
    glewInit();
    imguiInit(window);

    auto deferred_buffer = FrameBuffer(800, 600, {
        TextureFormat::RGB32F, // Position
        TextureFormat::RGB32F, // Normal
        TextureFormat::RGB32F, // Diffuse
        TextureFormat::RGB32F, // Specular
        TextureFormat::RGB32F, // Emissive
        TextureFormat::RGB32F, // Shininess
        TextureFormat::RGB32F, // Alpha
    });
    constexpr auto POSITION = 0;
    constexpr auto NORMAL = 1;
    constexpr auto DIFFUSE = 2;
    constexpr auto SPECULAR = 3;
    constexpr auto EMISSIVE = 4;
    constexpr auto SHININESS = 5;
    constexpr auto ALPHA = 6;

    auto deferred_shader = ShaderProgram(
        VertexShaderFile(std::ifstream("tests_assets/screen.vert")),
        FragmentShaderFile(std::ifstream("tests_assets/deferred.frag"))
    );

    auto camera = Camera(glm::perspective(glm::radians(70.0f), 320.0f / 240.0f, 0.1f, 100.0f));

    auto hdri_texture = loadTexture2D("tests_assets/hdri.jpg");
    auto uv_texture = loadTexture2D("tests_assets/uv.png");

    // auto material = PhongMaterial();
    //     material.diffuse_map = uv_texture;
    //     material.specular = ColorRGB(1.0f);
    //     material.shininess = 64.f;
    //     material.ambient_map = uv_texture;
    // auto& shader = material.shader();

    auto screen_geometry = plane(2, 2);
    auto screen_transform = Transform();
        screen_transform.position() = {0, 0, -1};

    auto shader = ShaderProgram(
        VertexShaderFile(std::ifstream("tests_assets/basic.vert")),
        FragmentShaderFile(std::ifstream("tests_assets/phong_pass.frag"))
    );
    ShaderProgram::use(shader);
    Texture2D::bindAs(uv_texture, 0);
    shader.uniform("u_diffuse", ColorRGB(1));
    shader.uniform("u_specular", ColorRGB(1));
    shader.uniform("u_emissive", ColorRGB(0));
    shader.uniform("u_shininess", 64.f);
    shader.uniform("u_opacity", 1.f);
    shader.uniform("u_use_diffuse_map", true);
    shader.uniform("u_use_specular_map", false);
    shader.uniform("u_use_emissive_map", false);
    shader.uniform("u_use_shininess_map", false);
    shader.uniform("u_use_opacity_map", false);
    shader.uniform("u_diffuse_map", 0);

    auto plane_model = cube(2, 2, 2);
    auto plane_transform = Transform();
        plane_transform.position() = {0, -2, -5};

    auto sphere_model = sphere(1, 32, 16);
    auto sphere_transform = Transform();
        sphere_transform.translate({0.0f, 0.0f, -5.0f});

    auto amb_light = Light(LightType::AMBIENT, ColorRGB(1.0f, 1.0f, 1.0f), 0.33333f);
    auto dir_light = Light(LightType::DIRECTIONAL, ColorRGB(1.0f, 1.0f, 1.0f), 1.0f);
        dir_light.position() = {0, 0, 0};
    auto pt_light = Light(LightType::POINT, ColorRGB(1.0f, 1.0f, 1.0f), 2.0f);
        pt_light.position() = {0, 2, -4};

    glm::vec3 dir_light_target = {0, 1, 1};

    auto light_helper_transform = Transform();
        light_helper_transform.scale() *= .1;

    auto start_time = std::chrono::high_resolution_clock::now();
    auto last_time = start_time;

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    while(!window.shouldClose()) {
        auto current_time = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration<float>(current_time - start_time).count();
        // auto delta_time = std::chrono::duration<float>(current_time - last_time).count();

        auto [width, height] = window.size();
        camera.projectionMatrix() = glm::perspective(glm::radians(70.0f), (float)width / height, 0.1f, 100.0f);

        Window::makeContextCurrent(window);
    
        // Renderer::bindRenderTarget(window);
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Renderer::drawHDRi(hdri_texture, camera);
        // GeometryBuffer::bind(plane_model);
        // ShaderProgram::use(shader);
        // material.apply();
        // Renderer::apply(shader, {dir_light, pt_light}, camera);
        // Renderer::apply(shader, plane_transform, camera);
        // Renderer::draw(plane_model);
        // GeometryBuffer::bind(sphere_model);
        // Renderer::apply(shader, sphere_transform, camera);
        // Renderer::draw(sphere_model);

        Renderer::bindRenderTarget(deferred_buffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Texture2D::bindAs(uv_texture, 0);
        ShaderProgram::use(shader);
        GeometryBuffer::bind(plane_model);
        Renderer::apply(shader, plane_transform, camera);
        Renderer::draw(plane_model);
        GeometryBuffer::bind(sphere_model);
        Renderer::apply(shader, sphere_transform, camera);
        Renderer::draw(sphere_model);

        Renderer::bindRenderTarget(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ShaderProgram::use(deferred_shader);
        GeometryBuffer::bind(screen_geometry);
        Texture2D::bindAs(hdri_texture, 0);
        Texture2D::bindAs(deferred_buffer.texture(POSITION), 1);
        Texture2D::bindAs(deferred_buffer.texture(NORMAL), 2);
        Texture2D::bindAs(deferred_buffer.texture(DIFFUSE), 3);
        Texture2D::bindAs(deferred_buffer.texture(SPECULAR), 4);
        Texture2D::bindAs(deferred_buffer.texture(EMISSIVE), 5);
        Texture2D::bindAs(deferred_buffer.texture(SHININESS), 6);
        Texture2D::bindAs(deferred_buffer.texture(ALPHA), 7);
        deferred_shader.uniform("u_hdri", 0);
        deferred_shader.uniform("u_position", 1);
        deferred_shader.uniform("u_normal", 2);
        deferred_shader.uniform("u_diffuse", 3);
        deferred_shader.uniform("u_specular", 4);
        deferred_shader.uniform("u_emissive", 5);
        deferred_shader.uniform("u_shininess", 6);
        deferred_shader.uniform("u_alpha", 7);
        Renderer::apply(deferred_shader, screen_transform, camera);
        Renderer::apply(deferred_shader, {dir_light, pt_light}, camera);
        Renderer::draw(screen_geometry);
        


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

        plane_transform.rotation() = glm::vec3(time);
        sphere_transform.position().x = glm::sin(time);
        sphere_transform.rotation().y = time;
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

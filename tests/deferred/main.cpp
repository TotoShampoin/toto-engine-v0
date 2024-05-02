#include "gfx-data.hpp"
#include <TotoEngine/TotoEngine.hpp>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

#include <AL/al.h>
#include <AL/alc.h>

#include <vector>
#include <chrono>

int test_deferred() {
    using TotoEngine::Graphics::TextureTarget::TEXTURE_2D;
    using TotoEngine::Graphics::ShaderType::VERTEX;
    using TotoEngine::Graphics::ShaderType::FRAGMENT;
    using TotoEngine::Graphics::LightType::AMBIENT;
    using TotoEngine::Graphics::LightType::DIRECTIONAL;
    using TotoEngine::Graphics::LightType::POINT;
    
    auto window = TotoEngine::Core::Window(800, 600, "TotoEngine");
    glewInit();

    auto& device = TotoEngine::Audio::DeviceManager::open();
    (void)TotoEngine::Audio::Context(device);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window.GLFWWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460");

    auto camera = TotoEngine::Graphics::Camera(glm::perspective(glm::radians(70.0f), 320.0f / 240.0f, 0.1f, 100.0f));
    auto deferred = DeferredRendering(800, 600);
    auto material = MyMaterial();
    auto screen_model = Mesh { TotoEngine::Graphics::plane(2, 2) };
    auto plane_model = Mesh { TotoEngine::Graphics::cube(2, 2, 2) };
    auto sphere_model = Mesh { TotoEngine::Graphics::sphere(1, 32, 16)  };
    auto amb_light = TotoEngine::Graphics::Light(AMBIENT, TotoEngine::Math::ColorRGB(1, 1, 1), 0.33333f);
    auto dir_light = TotoEngine::Graphics::Light(DIRECTIONAL, TotoEngine::Math::ColorRGB(1, 1, 1), 1);
    auto pt_light = TotoEngine::Graphics::Light(POINT, TotoEngine::Math::ColorRGB(1, 1, 1), 2);
    screen_model.transform.position() = {0, 0, -1};
    plane_model.transform.position() = {0, -2, -5};
    sphere_model.transform.position() = {0, 0, -5};
    dir_light.position() = {0, 0, 0};
    dir_light.lookAt({0, 1, 1});
    pt_light.position() = {0, 2, -4};

    auto sample = TotoEngine::Audio::loadSample("tests_assets/Bass-Drum-1.wav");

    auto start_time = std::chrono::high_resolution_clock::now();
    auto last_time = start_time;
    bool plays = false;

    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    while(!window.shouldClose()) {
        auto current_time = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration<float>(current_time - start_time).count();
        // auto delta_time = std::chrono::duration<float>(current_time - last_time).count();

        auto [width, height] = window.size();
        camera.projectionMatrix() = glm::perspective(glm::radians(70.0f), (float)width / height, 0.1f, 100.0f);

        TotoEngine::Core::Window::makeContextCurrent(window);

        renderDeferred(
            window, deferred, material,
            { plane_model, sphere_model }, { dir_light , pt_light },
            camera, screen_model
        );
        
        auto time_after_render = std::chrono::high_resolution_clock::now();
        auto render_time = std::chrono::duration<float>(time_after_render - current_time).count();

        imguiRender(render_time, plays, sample);

        TotoEngine::Core::Window::swapBuffers(window);
        TotoEngine::Core::Window::pollEvents();

        plane_model.transform.rotation() = glm::vec3(time);
        sphere_model.transform.position().x = glm::sin(time);
        sphere_model.transform.rotation().y = time;
        camera.lookAt({0, 0, -5});

        if(plays) {
            sample.play();
            plays = false;
        }

        last_time = current_time;
    }

    return 0;
}

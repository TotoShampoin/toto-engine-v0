#pragma once

#include "TotoEngine/Audio/Sample.hpp"
#include "TotoEngine/Graphics/GPUObjects/GeometryBuffer.hpp"
#include "TotoEngine/Graphics/Loaders/TextureLoader.hpp"
#include "TotoEngine/Graphics/RenderData/Camera.hpp"
#include "TotoEngine/Graphics/RenderData/Light.hpp"
#include "TotoEngine/Graphics/RenderData/Materials.hpp"
#include "TotoEngine/Math/Transform.hpp"
#include "TotoEngine/Core/Window.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <TotoEngine/Graphics/GPUObjects/Texture.hpp>
#include <TotoEngine/Graphics/GPUObjects/ShaderProgram.hpp>
#include <TotoEngine/TotoEngine.hpp>
#include <functional>
#include <vector>

namespace Test_Deferred {

struct DeferredRendering {
    DeferredRendering(int width, int height):
        buffer(width, height, {
            TotoEngine::Graphics::TextureFormat::RGB32F, // Position
            TotoEngine::Graphics::TextureFormat::RGB32F, // Normal
            TotoEngine::Graphics::TextureFormat::RGB32F, // Ambient
            TotoEngine::Graphics::TextureFormat::RGB32F, // Diffuse
            TotoEngine::Graphics::TextureFormat::RGB32F, // Specular
            TotoEngine::Graphics::TextureFormat::RGB32F, // Emissive
            TotoEngine::Graphics::TextureFormat::RGB32F, // Shininess
            TotoEngine::Graphics::TextureFormat::RGB32F, // Alpha
        }),
        shader(
            TotoEngine::Graphics::loadShaderFile<TotoEngine::Graphics::ShaderType::VERTEX>("tests_assets/screen.vert"),
            TotoEngine::Graphics::loadShaderFile<TotoEngine::Graphics::ShaderType::FRAGMENT>("tests_assets/deferred.frag")
        ),
        hdri(TotoEngine::Graphics::loadTexture2D("tests_assets/hdri.jpg"))
    {}

    void apply() {
        TotoEngine::Graphics::Texture2D::bindAs(hdri, 0);
        TotoEngine::Graphics::Texture2D::bindAs(buffer.texture(POSITION), POSITION + 1);
        TotoEngine::Graphics::Texture2D::bindAs(buffer.texture(NORMAL), NORMAL + 1);
        TotoEngine::Graphics::Texture2D::bindAs(buffer.texture(AMBIENT), AMBIENT + 1);
        TotoEngine::Graphics::Texture2D::bindAs(buffer.texture(DIFFUSE), DIFFUSE + 1);
        TotoEngine::Graphics::Texture2D::bindAs(buffer.texture(SPECULAR), SPECULAR + 1);
        TotoEngine::Graphics::Texture2D::bindAs(buffer.texture(EMISSIVE), EMISSIVE + 1);
        TotoEngine::Graphics::Texture2D::bindAs(buffer.texture(SHININESS), SHININESS + 1);
        TotoEngine::Graphics::Texture2D::bindAs(buffer.texture(ALPHA), ALPHA + 1);
        shader.uniform("u_hdri", 0);
        shader.uniform("u_position", POSITION + 1);
        shader.uniform("u_normal", NORMAL + 1);
        shader.uniform("u_ambient", AMBIENT + 1);
        shader.uniform("u_diffuse", DIFFUSE + 1);
        shader.uniform("u_specular", SPECULAR + 1);
        shader.uniform("u_emissive", EMISSIVE + 1);
        shader.uniform("u_shininess", SHININESS + 1);
        shader.uniform("u_alpha", ALPHA + 1);
    }        

    TotoEngine::Graphics::FrameBuffer buffer;
    TotoEngine::Graphics::ShaderProgram shader;
    TotoEngine::Graphics::Texture2D hdri;

    static constexpr auto POSITION = 0;
    static constexpr auto NORMAL = 1;
    static constexpr auto AMBIENT = 2;
    static constexpr auto DIFFUSE = 3;
    static constexpr auto SPECULAR = 4;
    static constexpr auto EMISSIVE = 5;
    static constexpr auto SHININESS = 6;
    static constexpr auto ALPHA = 7;
};

struct MyMaterial {
    MyMaterial():
        uv_texture(TotoEngine::Graphics::loadTexture2D("tests_assets/uv.png")),
        shader(
            TotoEngine::Graphics::loadShaderFile<TotoEngine::Graphics::ShaderType::VERTEX>("tests_assets/basic.vert"),
            TotoEngine::Graphics::loadShaderFile<TotoEngine::Graphics::ShaderType::FRAGMENT>("tests_assets/phong_pass.frag")
        )
    {
        material.diffuse_map = uv_texture;
        material.specular = TotoEngine::Math::ColorRGB(1.0f);
        material.shininess = 64.f;
        material.ambient_map = uv_texture;
    }

    void apply() {
        material.apply(shader);
    }

    TotoEngine::Graphics::Texture2D uv_texture;
    TotoEngine::Graphics::ShaderProgram shader;
    TotoEngine::Graphics::PhongMaterial material;
};

struct Mesh {
    TotoEngine::Graphics::GeometryBuffer model;
    TotoEngine::Math::Transform transform {};
};

inline void renderDeferred(
    TotoEngine::Core::Window& window,
    DeferredRendering& deferred, MyMaterial& material,
    const std::vector<std::reference_wrapper<Mesh>>& meshes,
    const std::vector<TotoEngine::Graphics::Light>& lights,
    TotoEngine::Graphics::Camera& camera,
    Mesh& screen
) {
    TotoEngine::Graphics::Renderer::bindRenderTarget(deferred.buffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    TotoEngine::Graphics::ShaderProgram::use(material.shader);
    material.apply();
    for(const Mesh& mesh : meshes) {
        TotoEngine::Graphics::GeometryBuffer::bind(mesh.model);
        TotoEngine::Graphics::Renderer::apply(material.shader, camera, mesh.transform);
        TotoEngine::Graphics::Renderer::draw(mesh.model);
    }

    TotoEngine::Graphics::Renderer::bindRenderTarget(window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    TotoEngine::Graphics::GeometryBuffer::bind(screen.model);
    TotoEngine::Graphics::ShaderProgram::use(deferred.shader);
    deferred.apply();
    TotoEngine::Graphics::Renderer::apply(deferred.shader, camera, screen.transform);
    TotoEngine::Graphics::Renderer::apply(deferred.shader, camera, lights);
    TotoEngine::Graphics::Renderer::draw(screen.model);
}

inline void imguiRender(
    float& render_time,
    bool& plays,
    TotoEngine::Audio::Sample& sample
) {
    static std::vector<float> render_times(200);
    render_times.push_back(1.f / render_time);
    if(render_times.size() > 200) {
        render_times.erase(render_times.begin());
    }
    std::string state;
    switch(sample.state()) {
        case AL_INITIAL:
            state = "INITIAL";
            break;
        case AL_PLAYING:
            state = "PLAYING";
            break;
        case AL_PAUSED:
            state = "PAUSED";
            break;
        case AL_STOPPED:
            state = "STOPPED";
            break;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(0, 0));
    ImGui::Begin("##", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus);
    ImGui::PlotHistogram("##fps", render_times.data(), render_times.size(), 0, nullptr, 0, std::max_element(render_times.begin(), render_times.end()).operator*(), ImVec2(400, 80));
    ImGui::Text("Render FPS: %.2f", 1 / render_time);
    plays |= ImGui::Button("Play");
    ImGui::SameLine();
    ImGui::Text("State: %s", state.c_str());
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

}

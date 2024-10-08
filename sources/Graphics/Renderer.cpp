#include "TotoEngine/Graphics/Renderer.hpp"

#include "TotoEngine/Graphics/GPUObjects/ShaderFile.hpp"
#include "TotoEngine/Graphics/GPUObjects/ShaderProgram.hpp"
#include "TotoEngine/Graphics/RenderData/Camera.hpp"
#include "impl/shaders/hdri.frag.hpp"
#include "impl/shaders/hdri.vert.hpp"
#include <GL/gl.h>

namespace TotoEngine {

namespace Graphics {

void Renderer::drawHDRi(const Texture2D& hdri_texture, const Camera& camera) {
    const auto& [hdri_model, hdri_shader] = HDRImodel();
    bind(hdri_model, hdri_shader);
    Texture2D::bindAs(hdri_texture, 0);
    hdri_shader.uniform("u_map", 0);
    apply(hdri_shader, camera);
    draw(hdri_model);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw(const GeometryBuffer& geometry_buffer, const GLenum& mode) {
    glDrawElements(mode, geometry_buffer.indexCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::bind(const GeometryBuffer& geometry_buffer, const ShaderProgram& shader) {
    GeometryBuffer::bind(geometry_buffer);
    ShaderProgram::use(shader);
}

void Renderer::bindRenderTarget(const FrameBuffer& target) {
    glBindFramebuffer(GL_FRAMEBUFFER, target.framebuffer());
    glViewport(0, 0, target.width(), target.height());
}
void Renderer::bindRenderTarget(const Core::Window& window) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    auto [width, height] = window.size();
    glViewport(0, 0, width, height);
}

void Renderer::apply(ShaderProgram& program, const Camera& camera) {
    program.uniform("u_projection", camera.projectionMatrix());
    program.uniform("u_view", camera.viewMatrix());
}
void Renderer::apply(ShaderProgram& program, const Camera& camera, const Math::Transform& transform) {
    apply(program, camera);
    program.uniform("u_model", transform.matrix());
}
void Renderer::apply(ShaderProgram& program, const Camera& camera, const std::vector<Light>& lights) {
    program.uniform("u_lights_count", static_cast<int>(lights.size()));
    for (size_t index = 0; index < lights.size(); index++) {
        auto& light = lights[index];
        program.uniform(std::format("u_lights[{}].color", index), light.color());
        program.uniform(std::format("u_lights[{}].intensity", index), light.intensity());
        program.uniform(std::format("u_lights[{}].type", index), static_cast<int>(light.type()));
        if (light.type() == LightType::POINT) {
            program.uniform(
                std::format("u_lights[{}].pos_or_dir", index),
                glm::vec3(camera.viewMatrix() * glm::vec4(light.position(), 1))
            );
        } else if (light.type() == LightType::DIRECTIONAL) {
            program.uniform(
                std::format("u_lights[{}].pos_or_dir", index), camera.viewNormalMatrix() * light.direction()
            );
        }
    }
}
void Renderer::apply(ShaderProgram& program, const std::function<void(ShaderProgram&)>& callback) {
    callback(program);
}

std::pair<GeometryBuffer&, ShaderProgram&> Renderer::HDRImodel() {
    static GeometryBuffer hdri_model = GeometryBuffer(
        {
            {{-1, -1, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
            { {1, -1, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
            {  {1, 1, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
            { {-1, 1, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
    },
        {
            0,
            1,
            2,
            0,
            2,
            3,
        }
    );
    static ShaderProgram hdri_shader = ShaderProgram(VertexShaderFile(hdri_vert), FragmentShaderFile(hdri_frag));
    return {hdri_model, hdri_shader};
}

void Renderer::clear(bool color, bool depth, bool stencil) {
    glClear(
        (color ? GL_COLOR_BUFFER_BIT : 0) | (depth ? GL_DEPTH_BUFFER_BIT : 0) | (stencil ? GL_STENCIL_BUFFER_BIT : 0)
    );
}
void Renderer::clearColor(TotoEngine::Math::ColorRGBA color) {
    glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::enable(GLenum cap) {
    glEnable(cap);
}

void Renderer::disable(GLenum cap) {
    glDisable(cap);
}

void Renderer::init() {
    glewInit();
}

} // namespace Graphics

} // namespace TotoEngine

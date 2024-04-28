#include "TotoEngine/Graphics/Renderer.hpp"

namespace TotoEngine {

void Renderer::draw(const GeometryBuffer &geometry_buffer) {
    glDrawElements(GL_TRIANGLES, geometry_buffer.indexCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::bind(const GeometryBuffer &geometry_buffer, const ShaderProgram &shader) {
    GeometryBuffer::bind(geometry_buffer);
    ShaderProgram::use(shader);
}

void Renderer::bindRenderTarget(const FrameBuffer &target) {
    glBindFramebuffer(GL_FRAMEBUFFER, target.framebuffer());
    glViewport(0, 0, target.width(), target.height());
}
void Renderer::bindRenderTarget(const Window &window) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    auto [width, height] = window.size();
    glViewport(0, 0, width, height);
}

void Renderer::apply(ShaderProgram& program, const std::vector<Light>& lights, const Camera& camera) {
    program.uniform("u_lights_count", static_cast<int>(lights.size()));
    for(size_t index = 0; index < lights.size(); index++) {
        auto& light = lights[index];
        program.uniform(std::format("u_lights[{}].color", index), light.color());
        program.uniform(std::format("u_lights[{}].intensity", index), light.intensity());
        program.uniform(std::format("u_lights[{}].type", index), static_cast<int>(light.type()));
        if(light.type() == LightType::POINT) {
            program.uniform(std::format("u_lights[{}].pos_or_dir", index),
                glm::vec3(camera.viewMatrix() * glm::vec4(light.position(), 1)));
        }
        else if(light.type() == LightType::DIRECTIONAL) {
            Vector3 direction = light.rotationMatrix() * Vector4(0.0f, 0.0f, 1.0f, 0.0f);
            program.uniform(std::format("u_lights[{}].pos_or_dir", index),
                camera.viewNormalMatrix() * direction);
        }
    }
}
void Renderer::apply(ShaderProgram& program, const Transform& transform, const Camera& camera) {
    apply(program, transform);
    apply(program, camera);
}
void Renderer::apply(ShaderProgram& program, const Camera& camera) {
    program.uniform("u_projection", camera.projectionMatrix());
    program.uniform("u_view", camera.viewMatrix());
}
void Renderer::apply(ShaderProgram& program, const Transform& transform) {
    program.uniform("u_model", transform.matrix());
}

}

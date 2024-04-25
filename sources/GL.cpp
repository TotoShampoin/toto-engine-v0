#include "TotoEngine/GL.hpp"

namespace TotoEngine {

void GL::init() {
    glewInit();
}

void GL::clear(const ClearMasks& masks) {
    glClear(
        (masks.color ? GL_COLOR_BUFFER_BIT : 0) |
        (masks.depth ? GL_DEPTH_BUFFER_BIT : 0) |
        (masks.stencil ? GL_STENCIL_BUFFER_BIT : 0)
    );
}
void GL::clearColor(const ColorRGBA& color) {
    auto& gl = instance();
    if(gl._clear_color != color) {
        glClearColor(color.r, color.g, color.b, color.a);
        gl._clear_color = color;
    }
}
void GL::enable(const GLenum& capability) {
    auto& gl = instance();
    if(gl._enabled_capabilities.find(capability) == gl._enabled_capabilities.end()) {
        gl._enabled_capabilities[capability] = false;
    }
    if(!gl._enabled_capabilities[capability]) {
        glEnable(capability);
        gl._enabled_capabilities[capability] = true;
    }
}
void GL::disable(const GLenum& capability) {
    auto& gl = instance();
    if(gl._enabled_capabilities.find(capability) == gl._enabled_capabilities.end()) {
        gl._enabled_capabilities[capability] = true;
    }
    if(gl._enabled_capabilities[capability]) {
        glDisable(capability);
        gl._enabled_capabilities[capability] = false;
    }
}

void GL::bindBuffer(const GLenum& target, const GLuint& buffer) {
    static GLuint bound_buffer = 0;
    if(bound_buffer == buffer) return;
    glBindBuffer(target, buffer);
    bound_buffer = buffer;
}

void GL::bindVertexArray(const GLuint& vao) {
    static GLuint bound_vao = 0;
    if(bound_vao == vao) return;
    glBindVertexArray(vao);
    bound_vao = vao;
}

void GL::useProgram(const GLuint& program) {
    static GLuint bound_program = 0;
    if(bound_program == program) return;
    glUseProgram(program);
    bound_program = program;
}

void GL::cullFace(const GLenum& face) {
    static GLenum bound_face = 0;
    if(bound_face == face) return;
    glCullFace(face);
}

void GL::draw(const GLenum& mode, const size_t& count, const size_t& indices) {
    glDrawElements(mode, count, GL_UNSIGNED_INT, reinterpret_cast<void*>(indices));
}

}

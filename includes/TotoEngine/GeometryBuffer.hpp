#pragma once

// #include <GL/glew.h>

#include "GL.hpp"
#include "Primitives.hpp"
#include <vector>

namespace TotoEngine {

class GeometryBuffer {
public:
    GeometryBuffer(const std::vector<Vertex>&, const std::vector<Index>&);
    ~GeometryBuffer();

    static void bind(const GeometryBuffer& buffer) {
        GL::bindVertexArray(buffer.vao());
    }
    static void unbind() {
        GL::bindVertexArray(0);
    }

    [[nodiscard]] GLuint vao() const { return _vao; }
    [[nodiscard]] GLuint vbo() const { return _vbo; }
    [[nodiscard]] GLuint ibo() const { return _ibo; }
    [[nodiscard]] const std::vector<Vertex>& vertices() const { return _vertices; }
    [[nodiscard]] const std::vector<GLuint>& indices() const { return _indices; }
private:
    GLuint _vbo;
    GLuint _ibo;
    GLuint _vao;
    std::vector<Vertex> _vertices;
    std::vector<Index> _indices;

    GLenum _cull_face = GL_NONE;
    GLenum _draw_mode = GL_TRIANGLES;
};

}

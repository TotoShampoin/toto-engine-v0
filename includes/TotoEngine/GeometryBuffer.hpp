#pragma once

// #include <GL/glew.h>

#include "GL.hpp"
#include "Primitives.hpp"
#include <vector>

namespace TotoEngine {

using GLBuffer = GLObject<
    [] { GLuint id; glGenBuffers(1, &id); return id; },
    [](GLuint& id) { glDeleteBuffers(1, &id); }
>;
using GLVertexArray = GLObject<
    [] { GLuint id; glGenVertexArrays(1, &id); return id; },
    [](GLuint& id) { glDeleteVertexArrays(1, &id); }
>;

class GeometryBuffer {
public:
    GeometryBuffer(const std::vector<Vertex>&, const std::vector<Index>&);
    ~GeometryBuffer() = default;

    static void bind(const GeometryBuffer& buffer) {
        glBindVertexArray(buffer.vao());
    }
    static void unbind() {
        glBindVertexArray(0);
    }

    [[nodiscard]] GLuint vao() const { return _vao; }
    [[nodiscard]] GLuint vbo() const { return _vbo; }
    [[nodiscard]] GLuint ibo() const { return _ibo; }
    [[nodiscard]] const std::vector<Vertex>& vertices() const { return _vertices; }
    [[nodiscard]] const std::vector<GLuint>& indices() const { return _indices; }
private:
    GLBuffer _vbo;
    GLBuffer _ibo;
    GLVertexArray _vao;
    std::vector<Vertex> _vertices;
    std::vector<Index> _indices;

    GLenum _cull_face = GL_NONE;
    GLenum _draw_mode = GL_TRIANGLES;
};

}

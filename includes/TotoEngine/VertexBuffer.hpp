#pragma once

#include <GL/glew.h>

#include "MeshData.hpp"
#include <vector>

namespace TotoEngine {

class VertexBuffer {
public:
    VertexBuffer(const std::vector<Vertex>&, const std::vector<Index>&);
    ~VertexBuffer();

    static void bind(const VertexBuffer& buffer) {
        bindVertexArray(buffer.vao());
    }
    static void unbind() {
        bindVertexArray(0);
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

    static void bindVertexArray(const GLuint& vao) {
        static GLuint bound_vao = 0;
        if(bound_vao == vao) return;
        glBindVertexArray(vao);
        bound_vao = vao;
    }
};

}

#include "TotoEngine/GeometryBuffer.hpp"
#include <vector>

namespace TotoEngine {

GeometryBuffer::GeometryBuffer(const std::vector<Vertex>& vertices, const std::vector<Index>& indices):
    _vertices(vertices), _indices(indices)
{
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ibo);
    glGenVertexArrays(1, &_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glEnableVertexAttribArray(VERTEX_ATTRIBUTE_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTRIBUTE_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTRIBUTE_UV);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glVertexAttribPointer(VERTEX_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glVertexAttribPointer(VERTEX_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glVertexAttribPointer(VERTEX_ATTRIBUTE_UV, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

GeometryBuffer::~GeometryBuffer() {
    glDeleteBuffers(1, &_ibo);
    glDeleteBuffers(1, &_vbo);
    glDeleteVertexArrays(1, &_vao);
}

void GeometryBuffer::draw(const GeometryBuffer& buffer) {
    glDrawElements(GL_TRIANGLES, buffer.indices().size(), GL_UNSIGNED_INT, nullptr);
}

void GeometryBuffer::bindVertexArray(const GLuint& vao) {
    static GLuint bound_vao = 0;
    if(bound_vao == vao) return;
    glBindVertexArray(vao);
    bound_vao = vao;
}

}

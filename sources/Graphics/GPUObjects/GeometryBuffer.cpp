#include "TotoEngine/Graphics/GPUObjects/GeometryBuffer.hpp"
#include <vector>

namespace TotoEngine {

namespace Graphics {

GeometryBuffer::GeometryBuffer():
    _vao(), _vbo(), _ibo(),
    _vertices(), _indices()
{
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

GeometryBuffer::GeometryBuffer(const std::vector<Vertex>& vertices, const std::vector<Index>& indices):
    GeometryBuffer()
{
    load(vertices, indices);
}

void GeometryBuffer::load(const std::vector<Vertex>& vertices, const std::vector<Index>& indices) {
    _vertices = vertices;
    _indices = indices;
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GeometryBuffer::bind(const optional_ref<const GeometryBuffer>& buffer) {
    if(!buffer.has_value()) {
        glBindVertexArray(0);
        return;
    }
    auto& buffer_ref = buffer.value().get();
    glBindVertexArray(buffer_ref.vao());
}

}

}

#include "TotoEngine/GeometryBuffer.hpp"
#include <vector>

namespace TotoEngine {

GeometryBuffer::GeometryBuffer(const std::vector<Vertex>& vertices, const std::vector<Index>& indices):
    _vao(), _vbo(), _ibo(),
    _vertices(vertices), _indices(indices)
{
    GL::bindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    GL::bindBuffer(GL_ARRAY_BUFFER, 0);
    GL::bindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);
    GL::bindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    GL::bindVertexArray(_vao);
    GL::bindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glEnableVertexAttribArray(VERTEX_ATTRIBUTE_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTRIBUTE_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTRIBUTE_UV);
    GL::bindBuffer(GL_ARRAY_BUFFER, _vbo);
    glVertexAttribPointer(VERTEX_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glVertexAttribPointer(VERTEX_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glVertexAttribPointer(VERTEX_ATTRIBUTE_UV, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    GL::bindBuffer(GL_ARRAY_BUFFER, 0);
    GL::bindVertexArray(0);
}

}

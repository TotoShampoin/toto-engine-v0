#pragma once

#include <GL/glew.h>

#include "GLObject.hpp"
#include "Aliases.hpp"
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

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};
static constexpr auto VERTEX_ATTRIBUTE_POSITION = 0;
static constexpr auto VERTEX_ATTRIBUTE_NORMAL = 1;
static constexpr auto VERTEX_ATTRIBUTE_UV = 2;
using Index = unsigned int;

class GeometryBuffer {
public:
    GeometryBuffer(const std::vector<Vertex>&, const std::vector<Index>&);
    ~GeometryBuffer() = default;

    constexpr static auto NONE = std::nullopt;
    static void bind(const optional_ref<GeometryBuffer>&);

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

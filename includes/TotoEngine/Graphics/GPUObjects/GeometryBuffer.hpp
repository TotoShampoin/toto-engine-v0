#pragma once

#include <GL/glew.h>

#include <TotoEngine/Aliases.hpp>
#include <TotoEngine/Primitives.hpp>
#include <TotoEngine/Instantiation.hpp>
#include <TotoEngine/LibObject.hpp>
#include <vector>

namespace TotoEngine {

using GLBuffer = LibObject<
    [] { GLuint id; glGenBuffers(1, &id); return id; },
    [](GLuint& id) { glDeleteBuffers(1, &id); }
>;
using GLVertexArray = LibObject<
    [] { GLuint id; glGenVertexArrays(1, &id); return id; },
    [](GLuint& id) { glDeleteVertexArrays(1, &id); }
>;

struct Vertex {
    Vector3 position;
    Vector3 normal;
    Vector2 uv;
};
static constexpr auto VERTEX_ATTRIBUTE_POSITION = 0;
static constexpr auto VERTEX_ATTRIBUTE_NORMAL = 1;
static constexpr auto VERTEX_ATTRIBUTE_UV = 2;
using Index = unsigned int;

class GeometryBuffer {
public:
    GeometryBuffer();
    GeometryBuffer(const std::vector<Vertex>&, const std::vector<Index>&);

    constexpr static auto NONE = std::nullopt;
    static void bind(const optional_ref<const GeometryBuffer>&);

    void load(const std::vector<Vertex>& vertices, const std::vector<Index>& indices);

    [[nodiscard]] size_t vertexCount() const { return _vertices.size(); }
    [[nodiscard]] size_t indexCount() const { return _indices.size(); }

    [[nodiscard]] GLuint vao() const { return _vao; }
    [[nodiscard]] GLuint vbo() const { return _vbo; }
    [[nodiscard]] GLuint ibo() const { return _ibo; }
    [[nodiscard]] const std::vector<Vertex>& vertices() const { return _vertices; }
    [[nodiscard]] const std::vector<GLuint>& indices() const { return _indices; }
    [[nodiscard]] GLenum cullFace() const { return _cull_face; }
    [[nodiscard]] GLenum drawMode() const { return _draw_mode; }
private:
    GLBuffer _vbo;
    GLBuffer _ibo;
    GLVertexArray _vao;
    std::vector<Vertex> _vertices;
    std::vector<Index> _indices;

    GLenum _cull_face = GL_NONE;
    GLenum _draw_mode = GL_TRIANGLES;
};

using GeometryBufferInstance = Manager<GeometryBuffer>::Instance;
using GeometryBufferManager = Manager<GeometryBuffer>;

}

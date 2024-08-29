#include <TotoEngine/Graphics/GPUObjects/Allocators.hpp>

namespace TotoEngine {

namespace Graphics {

GLuint createBuffer() {
    GLuint id;
    glGenBuffers(1, &id);
    return id;
}
void deleteBuffer(GLuint& id) {
    glDeleteBuffers(1, &id);
}
GLuint createVertexArray() {
    GLuint id;
    glGenVertexArrays(1, &id);
    return id;
}
void deleteVertexArray(GLuint& id) {
    glDeleteVertexArrays(1, &id);
}
GLuint createFrameBuffer() {
    GLuint b;
    glGenFramebuffers(1, &b);
    return b;
}
GLuint createRenderBuffer() {
    GLuint b;
    glGenRenderbuffers(1, &b);
    return b;
}
void deleteFrameBuffer(GLuint& b) {
    glDeleteFramebuffers(1, &b);
}
void deleteRenderBuffer(GLuint& b) {
    glDeleteRenderbuffers(1, &b);
}
GLuint createProgram() {
    return glCreateProgram();
}
void deleteProgram(GLuint& id) {
    glDeleteProgram(id);
}
GLuint createTexture() {
    GLuint id;
    glGenTextures(1, &id);
    return id;
}
void deleteTexture(GLuint& id) {
    glDeleteTextures(1, &id);
}

} // namespace Graphics

} // namespace TotoEngine

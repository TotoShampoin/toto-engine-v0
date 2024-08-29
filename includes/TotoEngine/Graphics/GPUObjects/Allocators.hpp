#pragma once

#include <GL/glew.h>

namespace TotoEngine {

namespace Graphics {

GLuint createBuffer();
void deleteBuffer(GLuint& id);
GLuint createVertexArray();
void deleteVertexArray(GLuint& id);
GLuint createFrameBuffer();
GLuint createRenderBuffer();
void deleteFrameBuffer(GLuint& b);
void deleteRenderBuffer(GLuint& b);
GLuint createProgram();
void deleteProgram(GLuint& id);
GLuint createTexture();
void deleteTexture(GLuint& id);

} // namespace Graphics

} // namespace TotoEngine

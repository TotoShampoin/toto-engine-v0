#pragma once

#include <TotoEngine/Graphics/GLObject.hpp>
#include <TotoEngine/Graphics/Texture.hpp>

namespace TotoEngine {

using GLFrameBuffer = GLObject<
    []() { GLuint b; glGenFramebuffers(1, &b); return b; },
    [](GLuint& b) { glDeleteFramebuffers(1, &b); }
>;
using GLRenderBuffer = GLObject<
    []() { GLuint b; glGenRenderbuffers(1, &b); return b; },
    [](GLuint& b) { glDeleteRenderbuffers(1, &b); }
>;

class FrameBuffer {
public:
    FrameBuffer(int width = 256, int height = 256, TextureFormat = TextureFormat::RGBA);

    void copyFrom(const FrameBuffer& other);

    [[nodiscard]] GLuint framebuffer() const { return _frame_buffer; }
    [[nodiscard]] GLuint renderbuffer() const { return _render_buffer; }
    [[nodiscard]] const Texture2D& texture() const { return _texture; }
    [[nodiscard]] int width() const { return _width; }
    [[nodiscard]] int height() const { return _height; }
private:
    GLFrameBuffer _frame_buffer;
    GLRenderBuffer _render_buffer;
    Texture2D _texture;

    int _width, _height;
};

}

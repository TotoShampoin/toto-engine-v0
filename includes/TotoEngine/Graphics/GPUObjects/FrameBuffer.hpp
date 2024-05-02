#pragma once

#include <TotoEngine/LibObject.hpp>
#include <TotoEngine/Graphics/GPUObjects/Texture.hpp>
#include <vector>

namespace TotoEngine {

namespace Graphics {

using GLFrameBuffer = LibObject<
    []() { GLuint b; glGenFramebuffers(1, &b); return b; },
    [](GLuint& b) { glDeleteFramebuffers(1, &b); }
>;
using GLRenderBuffer = LibObject<
    []() { GLuint b; glGenRenderbuffers(1, &b); return b; },
    [](GLuint& b) { glDeleteRenderbuffers(1, &b); }
>;

class FrameBuffer {
public:
    FrameBuffer(int width = 256, int height = 256, std::vector<TextureFormat> = {TextureFormat::RGBA});

    void copyFrom(const FrameBuffer& other);

    [[nodiscard]] GLuint framebuffer() const { return _frame_buffer; }
    [[nodiscard]] GLuint renderbuffer(int i = 0) const { return _render_buffers[i]; }
    [[nodiscard]] const Texture2D& texture(int i = 0) const { return _textures[i]; }
    [[nodiscard]] int width() const { return _width; }
    [[nodiscard]] int height() const { return _height; }
private:
    GLFrameBuffer _frame_buffer;
    std::vector<GLRenderBuffer> _render_buffers;
    std::vector<Texture2D> _textures;

    int _width, _height;
};

}

}

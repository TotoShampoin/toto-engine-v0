#include "TotoEngine/Graphics/FrameBuffer.hpp"
#include "TotoEngine/Graphics/Texture.hpp"
#include <GL/gl.h>
#include <format>

namespace TotoEngine {

TextureFormat getNonFloatFormat(TextureFormat format) {
    switch (format) {
        case TextureFormat::RGB32F: return TextureFormat::RGB;
        case TextureFormat::RGBA32F: return TextureFormat::RGBA;
        default: return format;
    }
}

FrameBuffer::FrameBuffer(int width, int height, TextureFormat format):
    _frame_buffer(), _render_buffer(),
    _texture(), _width(width), _height(height)
{
    glBindTexture(GL_TEXTURE_2D, _texture.texture());
    Texture2D::image<float>(_width, _height, format, getNonFloatFormat(format), nullptr);
    // Texture2D::storage(_width, _height, 5);
    glBindFramebuffer(GL_FRAMEBUFFER, _frame_buffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _render_buffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH32F_STENCIL8, _width, _height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _render_buffer);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture.texture(), 0);
    GLenum draw_buffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, draw_buffers);

    auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error(std::format("Framebuffer is not complete: {}", status));
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::copyFrom(const FrameBuffer& other) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, other._frame_buffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _frame_buffer);
    glBlitFramebuffer(0, 0, other._width, other._height, 0, 0, _width, _height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

}

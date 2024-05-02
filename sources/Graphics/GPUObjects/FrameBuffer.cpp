#include "TotoEngine/Graphics/GPUObjects/FrameBuffer.hpp"
#include "TotoEngine/Graphics/GPUObjects/Texture.hpp"
#include <GL/gl.h>
#include <format>

namespace TotoEngine {

namespace Graphics {

TextureFormat getNonFloatFormat(TextureFormat format) {
    switch (format) {
        case TextureFormat::RGB32F: return TextureFormat::RGB;
        case TextureFormat::RGBA32F: return TextureFormat::RGBA;
        default: return format;
    }
}

FrameBuffer::FrameBuffer(int width, int height, std::vector<TextureFormat> format):
    _frame_buffer(), _render_buffers(),
    _textures(), _width(width), _height(height)
{
    glBindFramebuffer(GL_FRAMEBUFFER, _frame_buffer);
    GLenum draw_buffers[format.size()];
    for (int i = 0; i < format.size(); i++) {
        _textures.emplace_back();
        _render_buffers.emplace_back();
        glBindTexture(GL_TEXTURE_2D, _textures[i].texture());
        Texture2D::image<float>(_width, _height, format[i], getNonFloatFormat(format[i]), nullptr);
        glBindRenderbuffer(GL_RENDERBUFFER, _render_buffers[i]);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH32F_STENCIL8, _width, _height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _render_buffers[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _textures[i].texture(), 0);
        draw_buffers[i] = GL_COLOR_ATTACHMENT0 + i;
    }
    glDrawBuffers(format.size(), draw_buffers);

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

}

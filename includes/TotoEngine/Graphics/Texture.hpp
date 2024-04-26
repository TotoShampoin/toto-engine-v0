#pragma once

#include "GLObject.hpp"
#include "TotoEngine/Graphics/Primitives.hpp"
#include <GL/gl.h>
#include <vector>

namespace TotoEngine {

using GLTexture = TotoEngine::GLObject<
    [] { GLuint id; glGenTextures(1, &id); return id; },
    [](GLuint& id) { glDeleteTextures(1, &id); }
>;

enum class TextureTarget {
    TEXTURE_2D = GL_TEXTURE_2D,
    TEXTURE_2D_ARRAY = GL_TEXTURE_2D_ARRAY,
    TEXTURE_3D = GL_TEXTURE_3D,
    TEXTURE_CUBE_MAP = GL_TEXTURE_CUBE_MAP
};

template <TextureTarget TARGET>
class Texture {
public:
    constexpr static auto NONE = std::nullopt;
    static void bind(const optional_ref<Texture<TARGET>>& texture) {
        if(!texture.has_value()) {
            glBindTexture(static_cast<GLenum>(TARGET), 0U);
            return;
        }
        auto& texture_ref = texture.value().get();
        glBindTexture(static_cast<GLenum>(TARGET), texture_ref.texture());
    }

    static void parameter(const GLenum& pname, const GLint& param) {
        glTexParameteri(static_cast<GLenum>(TARGET), pname, param);
    }
    static void parameter(const GLenum& pname, const GLfloat& param) {
        glTexParameterf(static_cast<GLenum>(TARGET), pname, param);
    }

    template<TextureTarget T = TARGET>
    typename std::enable_if<T == TextureTarget::TEXTURE_2D || T == TextureTarget::TEXTURE_CUBE_MAP>::type
    static image(const int& width, const int& height, const int& channels, const std::vector<uint8_t>& data) {
        GLuint format = getFormat(channels);
        glTexImage2D(static_cast<GLenum>(T), 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data.data());
    }
    template<TextureTarget T = TARGET>
    typename std::enable_if<T == TextureTarget::TEXTURE_2D_ARRAY || T == TextureTarget::TEXTURE_CUBE_MAP>::type
    static image(const int& width, const int& height, const int& depth, const int& channels, const std::vector<uint8_t>& data) {
        GLuint format = getFormat(channels);
        glTexImage3D(static_cast<GLenum>(T), 0, format, width, height, depth, 0, format, GL_UNSIGNED_BYTE, data.data());
    }

    [[nodiscard]] GLuint texture() const { return _texture; }

private:
    GLTexture _texture;

    static GLuint getFormat(const int& channels) {
        switch (channels) {
            case 1: return GL_RED;
            case 2: return GL_RG;
            case 3: return GL_RGB;
            case 4: return GL_RGBA;
            default: return GL_RGBA;
        }
    }
};

}

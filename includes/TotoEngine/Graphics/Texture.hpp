#pragma once

#include <TotoEngine/Graphics/GLObject.hpp>
#include <TotoEngine/Aliases.hpp>
#include <TotoEngine/Instantiation.hpp>
#include <GL/gl.h>

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
    Texture() {
        bind(*this);
        parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    constexpr static auto NONE = std::nullopt;
    static void bind(const optional_ref<const Texture<TARGET>>& texture) {
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

    static void generateMipmap() {
        glGenerateMipmap(static_cast<GLenum>(TARGET));
    }

    template<TextureTarget T = TARGET>
    typename std::enable_if<T == TextureTarget::TEXTURE_2D || T == TextureTarget::TEXTURE_CUBE_MAP>::type
    static image(const int& width, const int& height, const int& channels, const unsigned char* data) {
        GLuint format = getFormat(channels);
        glTexImage2D(static_cast<GLenum>(T), 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    }
    template<TextureTarget T = TARGET>
    typename std::enable_if<T == TextureTarget::TEXTURE_2D_ARRAY || T == TextureTarget::TEXTURE_CUBE_MAP>::type
    static image(const int& width, const int& height, const int& depth, const int& channels, const unsigned char* data) {
        GLuint format = getFormat(channels);
        glTexImage3D(static_cast<GLenum>(T), 0, format, width, height, depth, 0, format, GL_UNSIGNED_BYTE, data);
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

template <TextureTarget TARGET>
using TextureInstance = Manager<Texture<TARGET>>::Instance;
template <TextureTarget TARGET>
using TextureManager = Manager<Texture<TARGET>>;

using Texture2D = Texture<TextureTarget::TEXTURE_2D>;
using Texture2DInstance = Manager<Texture2D>::Instance;
using Texture2DManager = Manager<Texture2D>;

using Texture2DArray = Texture<TextureTarget::TEXTURE_2D_ARRAY>;
using Texture2DArrayInstance = Manager<Texture2DArray>::Instance;
using Texture2DArrayManager = Manager<Texture2DArray>;

using Texture3D = Texture<TextureTarget::TEXTURE_3D>;
using Texture3DInstance = Manager<Texture3D>::Instance;
using Texture3DManager = Manager<Texture3D>;

using TextureCubeMap = Texture<TextureTarget::TEXTURE_CUBE_MAP>;
using TextureCubeMapInstance = Manager<TextureCubeMap>::Instance;
using TextureCubeMapManager = Manager<TextureCubeMap>;

}

#pragma once

#include <TotoEngine/LibObject.hpp>
#include <TotoEngine/Aliases.hpp>
#include <TotoEngine/Instantiation.hpp>
#include <GL/gl.h>
#include <format>

namespace TotoEngine {

namespace Graphics {

using GLTexture = TotoEngine::LibObject<
    [] { GLuint id; glGenTextures(1, &id); return id; },
    [](GLuint& id) { glDeleteTextures(1, &id); }
>;

enum class TextureTarget {
    TEXTURE_2D = GL_TEXTURE_2D,
    TEXTURE_2D_ARRAY = GL_TEXTURE_2D_ARRAY,
    TEXTURE_3D = GL_TEXTURE_3D,
    TEXTURE_CUBE_MAP = GL_TEXTURE_CUBE_MAP
};

enum class TextureFormat {
    RED = GL_RED,
    RG = GL_RG,
    RGB = GL_RGB,
    RGBA = GL_RGBA,
    DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
    DEPTH_STENCIL = GL_DEPTH_STENCIL,
    R8 = GL_R8,
    R16 = GL_R16,
    R16F = GL_R16F,
    R32F = GL_R32F,
    RG8 = GL_RG8,
    RG16 = GL_RG16,
    RG16F = GL_RG16F,
    RG32F = GL_RG32F,
    RGB8 = GL_RGB8,
    RGB16 = GL_RGB16,
    RGB16F = GL_RGB16F,
    RGB32F = GL_RGB32F,
    RGBA8 = GL_RGBA8,
    RGBA16 = GL_RGBA16,
    RGBA16F = GL_RGBA16F,
    RGBA32F = GL_RGBA32F
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

    static void activeTexture(uint id) {
        glActiveTexture(GL_TEXTURE0 + id);
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

    static void bindAs(const optional_ref<const Texture<TARGET>>& texture, const uint& id) {
        activeTexture(id);
        bind(texture);
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

    template<typename T = uint8_t, TextureTarget TARGET_T = TARGET>
    typename std::enable_if<TARGET_T == TextureTarget::TEXTURE_2D || TARGET_T == TextureTarget::TEXTURE_CUBE_MAP>::type
    static image(const int& width, const int& height, const TextureFormat& in_format, const TextureFormat& out_format, const T* data) {
        glTexImage2D(static_cast<GLenum>(TARGET_T), 0, static_cast<GLenum>(in_format), width, height, 0, static_cast<GLenum>(out_format), getType<T>(), data);
        GLenum error = glGetError();
        if(error != GL_NO_ERROR) {
            throw std::runtime_error(std::format("Failed to create texture image: {}", (const char*)glewGetErrorString(error)));
        }
    }
    template<typename T = uint8_t, TextureTarget TARGET_T = TARGET>
    typename std::enable_if<TARGET_T == TextureTarget::TEXTURE_2D_ARRAY || TARGET_T == TextureTarget::TEXTURE_CUBE_MAP>::type
    static image(const int& width, const int& height, const int& depth, const TextureFormat& in_format, const TextureFormat& out_format, const T* data) {
        glTexImage3D(static_cast<GLenum>(TARGET_T), 0, in_format, width, height, depth, 0, out_format, getType<T>(), data);
        GLenum error = glGetError();
        if(error != GL_NO_ERROR) {
            throw std::runtime_error(std::format("Failed to create texture image: {}", (const char*)glewGetErrorString(error)));
        }
    }

    [[nodiscard]] GLuint texture() const { return _texture; }

private:
    GLTexture _texture;

    template<typename T>
    static GLuint getType() {
        if constexpr(std::is_same_v<T, uint8_t>) return GL_UNSIGNED_BYTE;
        if constexpr(std::is_same_v<T, uint16_t>) return GL_UNSIGNED_SHORT;
        if constexpr(std::is_same_v<T, uint32_t>) return GL_UNSIGNED_INT;
        if constexpr(std::is_same_v<T, float>) return GL_FLOAT;
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

}

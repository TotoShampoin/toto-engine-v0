#pragma once

#include "GLObject.hpp"
#include "TotoEngine/Primitives.hpp"
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
    constexpr static auto NONE = std::nullopt;
    static void bind(const optional_ref<Texture<TARGET>>& texture) {
        if(!texture.has_value()) {
            glBindTexture(static_cast<GLenum>(TARGET), 0U);
            return;
        }
        auto& texture_ref = texture.value().get();
        glBindTexture(static_cast<GLenum>(TARGET), texture_ref.texture());
    }

    [[nodiscard]] GLuint texture() const { return _texture; }

private:
    GLTexture _texture;
};

}

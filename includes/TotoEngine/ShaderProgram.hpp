#pragma once

#include <GL/glew.h>
#include <optional>

#include "ShaderFile.hpp"
#include "Primitives.hpp"

namespace TotoEngine {

class ShaderProgram {
public:
    template <typename... Shaders>
    ShaderProgram(const Shaders&... shaders) {
        static_assert((
            (std::is_base_of_v<ShaderFile<ShaderType::VERTEX>, Shaders> ||
            std::is_base_of_v<ShaderFile<ShaderType::FRAGMENT>, Shaders> ||
            std::is_base_of_v<ShaderFile<ShaderType::GEOMETRY>, Shaders> ||
            std::is_base_of_v<ShaderFile<ShaderType::TESS_CONTROL>, Shaders> ||
            std::is_base_of_v<ShaderFile<ShaderType::TESS_EVALUATION>, Shaders> ||
            std::is_base_of_v<ShaderFile<ShaderType::COMPUTE>, Shaders>)
            && ...));
        _program = glCreateProgram();
        (attachShader(shaders.shader()), ...);
        linkProgram();
    }
    ~ShaderProgram() {
        glDeleteProgram(_program);
    }

    constexpr static auto NONE = std::nullopt;
    static void use(const optional_ref<ShaderProgram>& program);

    [[nodiscard]] GLuint program() const { return _program; }
private:
    GLuint _program;

    void attachShader(const GLuint& shader);
    void linkProgram();
    static void useProgram(const GLuint& program);
};

}

#pragma once

#include "GL.hpp"
#include <optional>

#include "ShaderFile.hpp"
#include "Primitives.hpp"
#include "TotoEngine/Uniforms.hpp"

namespace TotoEngine {

class ShaderProgram {
public:
    template <typename... Shaders>
    ShaderProgram(const Shaders&... shaders):
        _program(glCreateProgram()),
        _uniform(_program)
    {
        static_assert((
            (std::is_base_of_v<ShaderFile<ShaderType::VERTEX>, Shaders> ||
            std::is_base_of_v<ShaderFile<ShaderType::FRAGMENT>, Shaders> ||
            std::is_base_of_v<ShaderFile<ShaderType::GEOMETRY>, Shaders> ||
            std::is_base_of_v<ShaderFile<ShaderType::TESS_CONTROL>, Shaders> ||
            std::is_base_of_v<ShaderFile<ShaderType::TESS_EVALUATION>, Shaders> ||
            std::is_base_of_v<ShaderFile<ShaderType::COMPUTE>, Shaders>)
            && ...));
        (attachShader(shaders.shader()), ...);
        linkProgram();
    }
    ~ShaderProgram() {
        glDeleteProgram(_program);
    }

    constexpr static auto NONE = std::nullopt;
    static void use(const optional_ref<ShaderProgram>& program);

    void uniform(const std::string& name, const UniformVariant& value);

    [[nodiscard]] GLuint program() const { return _program; }
    [[nodiscard]] Uniform& uniform() { return _uniform; }
private:
    GLuint _program;
    Uniform _uniform;

    // static GLuint& boundProgram();
    // static void useProgram(const GLuint& program);
    void attachShader(const GLuint& shader);
    void linkProgram();
};

}

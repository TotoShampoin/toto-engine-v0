#pragma once

#include <GL/glew.h>

#include "GLObject.hpp"
#include <optional>

#include "ShaderFile.hpp"
#include "Aliases.hpp"
#include "Uniforms.hpp"

namespace TotoEngine {

using GLProgram = GLObject<
    [] { return glCreateProgram(); },
    [](GLuint& id) { glDeleteProgram(id); }
>;

class ShaderProgram {
public:
    template <typename... Shaders>
    ShaderProgram(const Shaders&... shaders):
        _program(),
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
    ~ShaderProgram() = default;

    constexpr static auto NONE = std::nullopt;
    static void use(const optional_ref<ShaderProgram>& program);

    void uniform(const std::string& name, const UniformVariant& value);

    [[nodiscard]] GLuint program() const { return _program; }
    [[nodiscard]] Uniform& uniform() { return _uniform; }
private:
    GLProgram _program;
    Uniform _uniform;

    // static GLuint& boundProgram();
    // static void useProgram(const GLuint& program);
    void attachShader(const GLuint& shader);
    void linkProgram();
};

}

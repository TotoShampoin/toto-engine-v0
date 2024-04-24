#pragma once

#include <GL/glew.h>

#include "ShaderFile.hpp"

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
        (glAttachShader(_program, shaders.shader()), ...);
        glLinkProgram(_program);
        int success;
        char info_log[512];
        glGetProgramiv(_program, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(_program, 512, nullptr, info_log);
            std::cerr << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << info_log << std::endl;
        }
    }
    ~ShaderProgram() {
        glDeleteProgram(_program);
    }

    static void use(const ShaderProgram& program) {
        glUseProgram(program._program);
    }

    [[nodiscard]] GLuint program() const { return _program; }
private:
    GLuint _program;
};

}

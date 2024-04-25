#include "TotoEngine/ShaderProgram.hpp"
#include <format>
#include <stdexcept>

namespace TotoEngine {

void ShaderProgram::use(const std::optional<std::reference_wrapper<ShaderProgram>>& program) {
    if(program.has_value()) {
        useProgram(program.value().get().program());
    } else {
        useProgram(0);
    }
}

void ShaderProgram::useProgram(const GLuint& program) {
    static GLuint bound_program = 0;
    if(bound_program == program) return;
    glUseProgram(program);
    bound_program = program;
}

void ShaderProgram::attachShader(const GLuint& shader) {
    glAttachShader(_program, shader);
}
void ShaderProgram::linkProgram() {
    glLinkProgram(_program);
    int success;
    char info_log[512];
    glGetProgramiv(_program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(_program, 512, nullptr, info_log);
        throw std::runtime_error(std::format("ERROR::SHADER::PROGRAM::LINK_FAILED\n{}", info_log));
    }
}

}

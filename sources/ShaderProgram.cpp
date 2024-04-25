#include "TotoEngine/ShaderProgram.hpp"
#include <format>
#include <stdexcept>

namespace TotoEngine {

void ShaderProgram::use(const std::optional<std::reference_wrapper<ShaderProgram>>& program) {
    static GLuint bound_program = 0;
    if(!program.has_value()) {
        glUseProgram(0);
        return;
    }
    auto& program_ref = program.value().get();
    if(bound_program == program_ref.program()) {
        return;
    }
    glUseProgram(program_ref.program());
    bound_program = program_ref.program();
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

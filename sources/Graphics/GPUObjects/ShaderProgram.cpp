#include "TotoEngine/Graphics/GPUObjects/ShaderProgram.hpp"
#include "TotoEngine/Core/Aliases.hpp"
#include <format>
#include <stdexcept>

namespace TotoEngine {

namespace Graphics {

void ShaderProgram::use(const optional_ref<const ShaderProgram>& program) {
    if(!program.has_value()) {
        glUseProgram(0);
        return;
    }
    auto& program_ref = program.value().get();
    glUseProgram(program_ref.program());
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

void ShaderProgram::uniform(const std::string& name, const UniformVariant& value) {
    _uniform(name, value);
}

}

}

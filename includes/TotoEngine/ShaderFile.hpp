#pragma once

#include "GL.hpp"
#include <format>
#include <stdexcept>
#include <string>

namespace TotoEngine {

// Todo(Learning) Find out what geometry, tess_control and tess_evaluation shaders are
enum class ShaderType {
    VERTEX = GL_VERTEX_SHADER,
    FRAGMENT = GL_FRAGMENT_SHADER,
    GEOMETRY = GL_GEOMETRY_SHADER,
    TESS_CONTROL = GL_TESS_CONTROL_SHADER,
    TESS_EVALUATION = GL_TESS_EVALUATION_SHADER,
    COMPUTE = GL_COMPUTE_SHADER,
};
inline constexpr std::string shaderTypeToString(ShaderType type) {
    switch(type) {
        case ShaderType::VERTEX: return "VERTEX";
        case ShaderType::FRAGMENT: return "FRAGMENT";
        case ShaderType::GEOMETRY: return "GEOMETRY";
        case ShaderType::TESS_CONTROL: return "TESS_CONTROL";
        case ShaderType::TESS_EVALUATION: return "TESS_EVALUATION";
        case ShaderType::COMPUTE: return "COMPUTE";
    }
    return "UNKNOWN";
}

template <ShaderType TYPE>
class ShaderFile {
public:
    ShaderFile(const std::string& shader_source) {
        _shader = glCreateShader(static_cast<GLenum>(TYPE));
        const char* source = shader_source.c_str();
        glShaderSource(_shader, 1, &source, nullptr);
        glCompileShader(_shader);
        int success;
        char info_log[512];
        glGetShaderiv(_shader, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(_shader, 512, nullptr, info_log);
            throw std::runtime_error(std::format("ERROR::SHADER::{}::COMPILATION_FAILED\n{}", shaderTypeToString(TYPE), info_log));
        }
    }
    ~ShaderFile() {
        glDeleteShader(_shader);
    }

    [[nodiscard]] GLuint shader() const { return _shader; }
private:
    // TODO(GPU): Find a way to make this work with the GLObject class
    GLuint _shader;
};

using VertexShaderFile = ShaderFile<ShaderType::VERTEX>;
using FragmentShaderFile = ShaderFile<ShaderType::FRAGMENT>;
using GeometryShaderFile = ShaderFile<ShaderType::GEOMETRY>;
using TessControlShaderFile = ShaderFile<ShaderType::TESS_CONTROL>;
using TessEvaluationShaderFile = ShaderFile<ShaderType::TESS_EVALUATION>;
using ComputeShaderFile = ShaderFile<ShaderType::COMPUTE>;

}

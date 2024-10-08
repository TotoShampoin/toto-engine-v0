#pragma once

#include <GL/glew.h>

#include <TotoEngine/Math/Primitives.hpp>
#include <format>
#include <iostream>
#include <string>
#include <unordered_map>
#include <variant>

namespace TotoEngine {

namespace Graphics {

using UniformVariant = std::variant<
    bool, int, uint, float, double, Math::Vector2, Math::Vector3, Math::Vector4, Math::Matrix2, Math::Matrix3,
    Math::Matrix4>;

struct UniformVisitor {
    void operator()(const bool& value) const { glUniform1i(location, value); }
    void operator()(const GLint& value) const { glUniform1i(location, value); }
    void operator()(const GLuint& value) const { glUniform1ui(location, value); }
    void operator()(const GLfloat& value) const { glUniform1f(location, value); }
    void operator()(const GLdouble& value) const { glUniform1d(location, value); }
    void operator()(const Math::Vector2& value) const { glUniform2fv(location, 1, &value[0]); }
    void operator()(const Math::Vector3& value) const { glUniform3fv(location, 1, &value[0]); }
    void operator()(const Math::Vector4& value) const { glUniform4fv(location, 1, &value[0]); }
    void operator()(const Math::Matrix2& value) const { glUniformMatrix2fv(location, 1, GL_FALSE, &value[0][0]); }
    void operator()(const Math::Matrix3& value) const { glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]); }
    void operator()(const Math::Matrix4& value) const { glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]); }

    GLint location;
};

class Uniform {
private:
    Uniform(const GLint& program)
        : _program(program) {}
    ~Uniform() = default;

    void operator()(const std::string& name, const UniformVariant& value) {
        if (_uniforms.find(name) == _uniforms.end()) {
            GLint location = glGetUniformLocation(_program, name.c_str());
            if (location == -1) {
                std::cerr << std::format("Warning: Uniform '{}' not found in shader program\n", name);
                // throw std::runtime_error(std::format("ERROR::SHADER::UNIFORM::NOT_FOUND ({})", name));
            }
            _uniforms[name] = {location};
        }
        std::visit(_uniforms.at(name), value);
    }

    const GLint _program;
    std::unordered_map<std::string, UniformVisitor> _uniforms;

    friend class ShaderProgram;
};

} // namespace Graphics

} // namespace TotoEngine

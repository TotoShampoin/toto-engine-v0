#pragma once

#include <TotoEngine/Graphics/ShaderFile.hpp>
#include <filesystem>
#include <fstream>

namespace TotoEngine {

template <ShaderType TYPE>
ShaderFile<TYPE> loadShaderFile(const std::filesystem::path& path) {
    auto file = std::ifstream(path);
    if (!file.is_open()) {
        throw std::runtime_error(std::format("Failed to open shader file: {}", path.string()));
    }
    std::string shader_source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return ShaderFile<TYPE>(shader_source);
}

}

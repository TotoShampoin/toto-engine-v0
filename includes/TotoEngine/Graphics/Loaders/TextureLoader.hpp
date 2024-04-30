#pragma once

#include <TotoEngine/Graphics/GPUObjects/Texture.hpp>
#include <filesystem>

namespace TotoEngine {

Texture2D loadTexture2D(const std::filesystem::path& path, const bool& flip = true, const bool& generate_mipmap = true);

}

#include "TotoEngine/Graphics/Loaders/TextureLoader.hpp"
#include "TotoEngine/Graphics/GPUObjects/Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace TotoEngine {

namespace Graphics {

Texture2D loadTexture2D(const std::filesystem::path& path, const bool& flip, const bool& generate_mipmap) {
    Texture2D texture;
    Texture2D::bind(texture);
    Texture2D::parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    Texture2D::parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    Texture2D::parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    Texture2D::parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    if(flip)
        stbi_set_flip_vertically_on_load(true);
    int width, height, channels;
    auto data = stbi_load(path.string().c_str(), &width, &height, &channels, 0);
    TextureFormat format;
    switch(channels) {
        case 1: format = TextureFormat::RED; break;
        case 2: format = TextureFormat::RG; break;
        case 3: format = TextureFormat::RGB; break;
        case 4: format = TextureFormat::RGBA; break;
        default: throw std::runtime_error("Unsupported number of channels");
    }
    Texture2D::image(width, height, format, format, data);
    stbi_image_free(data);
    if(generate_mipmap)
        Texture2D::generateMipmap();
    Texture2D::bind(Texture2D::NONE);
    return texture;
}

}

}

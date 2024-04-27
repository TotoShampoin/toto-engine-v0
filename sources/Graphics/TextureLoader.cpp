#include "TotoEngine/Graphics/TextureLoader.hpp"
#include "TotoEngine/Graphics/Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace TotoEngine {

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
    Texture2D::image(width, height, channels, data);
    stbi_image_free(data);
    if(generate_mipmap)
        Texture2D::generateMipmap();
    Texture2D::bind(Texture2D::NONE);
    return texture;
}

}

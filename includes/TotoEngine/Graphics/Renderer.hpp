#pragma once

#include <TotoEngine/Graphics/GPUObjects/GeometryBuffer.hpp>
#include <TotoEngine/Graphics/GPUObjects/ShaderProgram.hpp>
#include <TotoEngine/Graphics/GPUObjects/FrameBuffer.hpp>
#include <TotoEngine/Graphics/GPUObjects/Texture.hpp>
#include <TotoEngine/Graphics/RenderData/Camera.hpp>
#include <TotoEngine/Graphics/RenderData/Light.hpp>
#include <TotoEngine/Primitives.hpp>
#include <TotoEngine/Transform.hpp>
#include <TotoEngine/Window.hpp>
#include <functional>
#include <utility>
#include <vector>

namespace TotoEngine {

namespace Graphics {

class Renderer {
public:
    static void drawHDRi(const Texture2D&, const Camera&);

    static void draw(const GeometryBuffer& geometry_buffer);

    static void bind(const GeometryBuffer& geometry_buffer, const ShaderProgram& shader);

    static void bindRenderTarget(const FrameBuffer& target);
    static void bindRenderTarget(const Window& window);

    static void apply(ShaderProgram&, const Camera&);
    static void apply(ShaderProgram&, const Camera&, const Transform&);
    static void apply(ShaderProgram&, const Camera&, const std::vector<Light>&);
    static void apply(ShaderProgram&, const std::function<void(ShaderProgram&)>&);
private:
    static std::pair<GeometryBuffer&, ShaderProgram&> HDRImodel();
};

}

}

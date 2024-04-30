#pragma once

#include <TotoEngine/Graphics/Camera.hpp>
#include <TotoEngine/Graphics/GeometryBuffer.hpp>
#include <TotoEngine/Graphics/Light.hpp>
#include <TotoEngine/Graphics/ShaderProgram.hpp>
#include <TotoEngine/Graphics/FrameBuffer.hpp>
#include <TotoEngine/Primitives.hpp>
#include <TotoEngine/Transform.hpp>
#include "TotoEngine/Window.hpp"
#include <functional>
#include <utility>
#include <vector>

namespace TotoEngine {

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

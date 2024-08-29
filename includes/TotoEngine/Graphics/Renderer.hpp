#pragma once

#include <GL/glew.h>

#include <TotoEngine/Core/Window.hpp>
#include <TotoEngine/Graphics/GPUObjects/FrameBuffer.hpp>
#include <TotoEngine/Graphics/GPUObjects/GeometryBuffer.hpp>
#include <TotoEngine/Graphics/GPUObjects/ShaderProgram.hpp>
#include <TotoEngine/Graphics/GPUObjects/Texture.hpp>
#include <TotoEngine/Graphics/RenderData/Camera.hpp>
#include <TotoEngine/Graphics/RenderData/Light.hpp>
#include <TotoEngine/Math/Primitives.hpp>
#include <TotoEngine/Math/Transform.hpp>
#include <functional>
#include <utility>
#include <vector>

namespace TotoEngine {

namespace Graphics {

class Renderer {
public:
    static void drawHDRi(const Texture2D&, const Camera&);
    static void draw(const GeometryBuffer& geometry_buffer, const GLenum& mode = GL_TRIANGLES);

    static void bind(const GeometryBuffer& geometry_buffer, const ShaderProgram& shader);
    static void bindRenderTarget(const FrameBuffer& target);
    static void bindRenderTarget(const Core::Window& window);

    static void apply(ShaderProgram&, const Camera&);
    static void apply(ShaderProgram&, const Camera&, const Math::Transform&);
    static void apply(ShaderProgram&, const Camera&, const std::vector<Light>&);
    static void apply(ShaderProgram&, const std::function<void(ShaderProgram&)>&);

    static void clear(bool color = true, bool depth = true, bool stencil = false);
    static void clearColor(TotoEngine::Math::ColorRGBA color = {0, 0, 0, 1});
    static void enable(GLenum);
    static void disable(GLenum);

    static void init();

private:
    static std::pair<GeometryBuffer&, ShaderProgram&> HDRImodel();
};

} // namespace Graphics

} // namespace TotoEngine

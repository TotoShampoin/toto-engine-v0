#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <TotoEngine/Window.hpp>
#include <TotoEngine/Primitives.hpp>
#include <TotoEngine/Transform.hpp>
#include <TotoEngine/Graphics/GeometryBuffer.hpp>
#include <TotoEngine/Graphics/Shapes.hpp>
#include <TotoEngine/Graphics/ShaderProgram.hpp>
#include <TotoEngine/Graphics/Materials.hpp>
#include <TotoEngine/Graphics/Uniforms.hpp>
#include <TotoEngine/Graphics/Texture.hpp>
#include <TotoEngine/Graphics/TextureLoader.hpp>
#include <TotoEngine/Graphics/ShaderLoader.hpp>
#include <TotoEngine/Graphics/Light.hpp>
#include <TotoEngine/Graphics/Camera.hpp>
#include <TotoEngine/Graphics/Renderer.hpp>
#include <TotoEngine/Graphics/FrameBuffer.hpp>

#define __TOTO_ENGINE_MAJOR__ 0
#define __TOTO_ENGINE_MINOR__ 0
#define __TOTO_ENGINE_PATCH__ 0

namespace TotoEngine {

void printInfo();

};

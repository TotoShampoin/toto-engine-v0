#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <TotoEngine/Window.hpp>
#include <TotoEngine/Primitives.hpp>
#include <TotoEngine/Transform.hpp>

#include <TotoEngine/Graphics/Renderer.hpp>
#include <TotoEngine/Graphics/RenderData/Materials.hpp>
#include <TotoEngine/Graphics/RenderData/Light.hpp>
#include <TotoEngine/Graphics/RenderData/Camera.hpp>
#include <TotoEngine/Graphics/GPUObjects/GeometryBuffer.hpp>
#include <TotoEngine/Graphics/GPUObjects/ShaderProgram.hpp>
#include <TotoEngine/Graphics/GPUObjects/Uniforms.hpp>
#include <TotoEngine/Graphics/GPUObjects/Texture.hpp>
#include <TotoEngine/Graphics/GPUObjects/FrameBuffer.hpp>
#include <TotoEngine/Graphics/Loaders/Shapes.hpp>
#include <TotoEngine/Graphics/Loaders/TextureLoader.hpp>
#include <TotoEngine/Graphics/Loaders/ShaderLoader.hpp>

#include <TotoEngine/Audio/Device.hpp>
#include <TotoEngine/Audio/Context.hpp>
#include <TotoEngine/Audio/Sample.hpp>
#include <TotoEngine/Audio/SampleLoader.hpp>

#define __TOTO_ENGINE_MAJOR__ 0
#define __TOTO_ENGINE_MINOR__ 0
#define __TOTO_ENGINE_PATCH__ 0

namespace TotoEngine {

void printInfo();

};

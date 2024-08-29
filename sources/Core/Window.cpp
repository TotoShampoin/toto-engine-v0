#include "TotoEngine/Core/Window.hpp"
#include <stdexcept>

namespace TotoEngine {

namespace Core {

void Window::init() {
    static bool initialized = false;
    if (initialized) {
        return;
    }
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }
    initialized = true;
}

void Window::terminate() {
    glfwTerminate();
}

Window::Window(int width, int height, const char* title) {
    Window::init();
    _glfw_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!_glfw_window) {
        terminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    Window::makeContextCurrent(*this);
}

Window::~Window() {
    glfwDestroyWindow(_glfw_window);
}

} // namespace Core

} // namespace TotoEngine

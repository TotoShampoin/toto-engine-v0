#include "TotoEngine/io/Window.hpp"
#include <stdexcept>

namespace TotoEngine {
namespace IO {

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
    _glfw_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!_glfw_window) {
        terminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
}

Window::~Window() {
    glfwDestroyWindow(_glfw_window);
}

}
}

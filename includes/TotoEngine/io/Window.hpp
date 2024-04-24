#pragma once

#include <GLFW/glfw3.h>
#include <utility>

namespace TotoEngine {
namespace IO {

class Window {
public:
    Window(int width, int height, const char* title);
    ~Window();

    [[nodiscard]] GLFWwindow* GLFWWindow() { return _glfw_window; }

    [[nodiscard]] std::pair<int, int> size() const {
        int width, height;
        glfwGetWindowSize(_glfw_window, &width, &height);
        return {width, height};
    }

    [[nodiscard]] bool shouldClose() const { return glfwWindowShouldClose(_glfw_window); }
    void makeContextCurrent() { glfwMakeContextCurrent(_glfw_window); }
    void swapBuffers() { glfwSwapBuffers(_glfw_window); }
    static void pollEvents() { glfwPollEvents(); }

    static void init();
    static void terminate();

private:
    GLFWwindow* _glfw_window;
};

}
}

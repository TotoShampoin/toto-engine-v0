#include "TotoEngine/TotoEngine.hpp"
#include <GLFW/glfw3.h>

int main(int /* argc */, const char* /* argv */[]) {
    TotoEngine::printInfo();
    TotoEngine::Window::init();

    TotoEngine::Window window(800, 600, "TotoEngine");

    while(!window.shouldClose()) {
        window.makeContextCurrent();

        window.pollEvents();
        window.swapBuffers();
    }

    return 0;
}

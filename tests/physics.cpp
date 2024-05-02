#include <TotoEngine/TotoEngine.hpp>

int test_physics() {
    auto window = TotoEngine::Core::Window(800, 600, "Doing Physics");
    TotoEngine::Graphics::Renderer::init();

    while(!window.shouldClose()) {
        TotoEngine::Graphics::Renderer::clear();

        TotoEngine::Core::Window::pollEvents();
        TotoEngine::Core::Window::swapBuffers(window);
    }

    return 0;
}
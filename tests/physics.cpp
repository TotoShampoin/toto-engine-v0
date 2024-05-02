#include <TotoEngine/TotoEngine.hpp>

namespace Test_Physics {

void render(TotoEngine::Core::Window& window) {
    TotoEngine::Graphics::Renderer::clear();

    TotoEngine::Core::Window::swapBuffers(window);
}

int test_physics() {
    auto window = TotoEngine::Core::Window(800, 600, "Doing Physics");
    TotoEngine::Graphics::Renderer::init();

    while(!window.shouldClose()) {
        render(window);
        TotoEngine::Core::Window::pollEvents();
    }

    return 0;
}

}

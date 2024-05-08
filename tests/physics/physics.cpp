#include <TotoEngine/TotoEngine.hpp>

namespace Test_Physics {

using namespace TotoEngine;

int test_physics() {
    auto window = Core::Window(800, 600, "Doing Physics");
    Graphics::Renderer::init();

    Core::Logger::init();
    auto logs = std::vector<std::string>();
    Core::Logger::setLogger([&logs](const char* message) {
        logs.push_back(std::format("[I] {}", message));
        if(logs.size() > 10)
            logs.erase(logs.begin());
    });
    Core::Logger::setWarningLogger([&logs](const char* message) {
        logs.push_back(std::format("[W] {}", message));
        if(logs.size() > 10)
            logs.erase(logs.begin());
    });
    Core::Logger::setErrorLogger([&logs](const char* message) {
        logs.push_back(std::format("[E] {}", message));
        if(logs.size() > 10)
            logs.erase(logs.begin());
    });

    while(!window.shouldClose()) {
        Graphics::Renderer::clear();

        // 

        Core::Window::swapBuffers(window);
        Core::Window::pollEvents();
    }

    return 0;
}

}

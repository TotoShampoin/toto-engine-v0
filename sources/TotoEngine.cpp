#include "TotoEngine/TotoEngine.hpp"
#include <iostream>

namespace TotoEngine {

void printInfo() {
    std::cout << "TotoEngine version " << __TOTO_ENGINE_MAJOR__ << "." << __TOTO_ENGINE_MINOR__ << "." << __TOTO_ENGINE_PATCH__ << "\n";
    std::cout << "Currently empty" << "\n";
}

}

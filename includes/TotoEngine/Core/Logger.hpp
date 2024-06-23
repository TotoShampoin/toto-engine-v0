#pragma once

#include <functional>
#include <iostream>

namespace TotoEngine {

namespace Core {

class Logger {
public:
    static void init() {
        (void)getLogger();
    }

    static void log(const char* message) {
        getLogger().logger(message);
    }
    static void warn(const char* message) {
        getLogger().warning_logger(message);
    }
    static void error(const char* message) {
        getLogger().error_logger(message);
    }

    static void setLogger(std::function<void(const char*)> logger) {
        getLogger().logger = logger;
    }
    static void setWarningLogger(std::function<void(const char*)> logger) {
        getLogger().warning_logger = logger;
    }
    static void setErrorLogger(std::function<void(const char*)> logger) {
        getLogger().error_logger = logger;
    }

private:
    std::function<void(const char*)> logger = [](const char* message) { std::cout << message << std::endl; };
    std::function<void(const char*)> warning_logger = [](const char* message) { std::cout << "Warning: " << message << std::endl; };
    std::function<void(const char*)> error_logger = [](const char* message) { std::cout << "Error: " << message << std::endl; };

    Logger() = default;
    static Logger& getLogger() {
        static Logger logger;
        return logger;
    }
};

}

}

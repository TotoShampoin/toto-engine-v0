#pragma once 

#include "Device.hpp"
#include <AL/al.h>
#include <AL/alc.h>
#include <iostream>

namespace TotoEngine {

namespace Audio {

class Context {
public:
    Context(const Device& device):
        _context(alcCreateContext(device.device(), NULL))
    {
        Context::makeCurrent(*this);
        if(!(_eax2_support = alIsExtensionPresent("EAX2.0"))) {
            std::cerr << "WARNING: EAX 2.0 is not present\n";
        }
    }

    static void makeCurrent(const Context& context) {
        alcMakeContextCurrent(context._context);
    }

    ALCcontext* context() const { return _context; }
private:
    ALCcontext* _context;
    bool _eax2_support;
};

}

}

#include "TotoEngine/GL.hpp"

namespace TotoEngine {

void GL::init() {
    glewInit();
}

void GL::clear(const ClearMasks& masks) {
    glClear(
        (masks.color ? GL_COLOR_BUFFER_BIT : 0) |
        (masks.depth ? GL_DEPTH_BUFFER_BIT : 0) |
        (masks.stencil ? GL_STENCIL_BUFFER_BIT : 0)
    );
}
void GL::clearColor(const ColorRGBA& color) {
    auto& gl = instance();
    if(gl._clear_color != color) {
        glClearColor(color.r, color.g, color.b, color.a);
        gl._clear_color = color;
    }
}
void GL::enable(const GLenum& capability) {
    auto& gl = instance();
    if(gl._enabled_capabilities.find(capability) == gl._enabled_capabilities.end()) {
        gl._enabled_capabilities[capability] = false;
    }
    if(!gl._enabled_capabilities[capability]) {
        glEnable(capability);
        gl._enabled_capabilities[capability] = true;
    }
}
void GL::disable(const GLenum& capability) {
    auto& gl = instance();
    if(gl._enabled_capabilities.find(capability) == gl._enabled_capabilities.end()) {
        gl._enabled_capabilities[capability] = true;
    }
    if(gl._enabled_capabilities[capability]) {
        glDisable(capability);
        gl._enabled_capabilities[capability] = false;
    }
}

}

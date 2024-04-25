#include "TotoEngine/GL.hpp"

namespace TotoEngine {
namespace GL {

void init() {
    glewInit();
}

void clear(const ClearMasks& masks) {
    glClear(
        (masks.color ? GL_COLOR_BUFFER_BIT : 0) |
        (masks.depth ? GL_DEPTH_BUFFER_BIT : 0) |
        (masks.stencil ? GL_STENCIL_BUFFER_BIT : 0)
    );
}
void clearColor(const ColorRGBA& color) {
    glClearColor(color.r, color.g, color.b, color.a);
}

void draw(const GLenum& mode, const size_t& count, const size_t& indices) {
    glDrawElements(mode, count, GL_UNSIGNED_INT, reinterpret_cast<void*>(indices));
}

}
}

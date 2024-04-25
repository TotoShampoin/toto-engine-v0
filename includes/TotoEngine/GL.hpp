#pragma once

#include <GL/glew.h>

#include "TotoEngine/Primitives.hpp"

namespace TotoEngine {
namespace GL {

void init();
struct ClearMasks {
    bool color = true;
    bool depth = true;
    bool stencil = false;
};
void clear(const ClearMasks&);
void clearColor(const ColorRGBA&);

void draw(const GLenum& mode, const size_t& count, const size_t& indices = 0);

};

template <GLuint gen(), void del(GLuint&)>
class GLObject {
public:
    GLObject():
        _id(gen())
    {}
    ~GLObject() {
        del(_id);
    }
    GLObject(const GLObject&) = delete;
    GLObject(GLObject&&) = delete;
    GLObject& operator=(const GLObject&) = delete;
    GLObject&& operator=(GLObject&&) = delete;

    [[nodiscard]] GLuint id() const { return _id; }
    [[nodiscard]] operator GLuint() const { return _id; }
private:
    GLuint _id;
};

}

#pragma once

#include <GL/glew.h>

namespace TotoEngine {

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

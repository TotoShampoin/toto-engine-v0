#pragma once

#include <GL/glew.h>
#include <algorithm>

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
    GLObject(GLObject&& other):
        _id(other._id) {
        other._id = 0;
    }
    GLObject&& operator=(GLObject&& other) {
        if(this != &other) {
            del(_id);
            _id = other._id;
            other._id = 0;
        }
        return std::move(*this);
    }

    [[nodiscard]] GLuint id() const { return _id; }
    [[nodiscard]] operator GLuint() const { return _id; }

    GLObject(const GLObject&) = delete;
    GLObject& operator=(const GLObject&) = delete;
private:
    GLuint _id;
};

}

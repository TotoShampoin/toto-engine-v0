#pragma once

#include <GL/glew.h>
#include <algorithm>

namespace TotoEngine {

namespace Core {

template <GLuint gen(), void del(GLuint&)>
class LibObject {
public:
    LibObject():
        _id(gen())
    {}
    ~LibObject() {
        del(_id);
    }
    LibObject(LibObject&& other):
        _id(other._id) {
        other._id = 0;
    }
    LibObject&& operator=(LibObject&& other) {
        if(this != &other) {
            del(_id);
            _id = other._id;
            other._id = 0;
        }
        return std::move(*this);
    }

    [[nodiscard]] GLuint id() const { return _id; }
    [[nodiscard]] operator GLuint() const { return _id; }

    LibObject(const LibObject&) = delete;
    LibObject& operator=(const LibObject&) = delete;
private:
    GLuint _id;
};

}

}

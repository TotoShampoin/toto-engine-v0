#pragma once

#include <GL/glew.h>
#include <unordered_map>

#include "TotoEngine/Primitives.hpp"

namespace TotoEngine {

class GL {
public:
    static void init();
    struct ClearMasks {
        bool color = true;
        bool depth = true;
        bool stencil = false;
    };
    static void clear(const ClearMasks&);
    static void clearColor(const ColorRGBA&);
    static void enable(const GLenum&);
    static void disable(const GLenum&);

    static void bindBuffer(const GLenum& target, const GLuint& buffer);
    static void bindVertexArray(const GLuint& vao);
    static void useProgram(const GLuint& program);

    static void cullFace(const GLenum&);
    static void draw(const GLenum& mode, const size_t& count, const size_t& indices = 0);

    static GL& instance() {
        static GL instance;
        return instance;
    }
    ~GL() = default;
    GL(const GL&) = delete;
    GL(GL&&) = delete;
    GL& operator=(const GL&) = delete;
    GL&& operator=(GL&&) = delete;
private:
    GL() = default;
    ColorRGBA _clear_color {0};
    std::unordered_map<GLenum, bool> _enabled_capabilities;
};

}

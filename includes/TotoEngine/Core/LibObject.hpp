#pragma once

#include <algorithm>

namespace TotoEngine {

namespace Core {

template <uint gen(), void del(uint&)>
class LibObject {
public:
    LibObject()
        : _id(gen()) {}
    ~LibObject() { del(_id); }
    LibObject(LibObject&& other)
        : _id(other._id) {
        other._id = 0;
    }
    LibObject&& operator=(LibObject&& other) {
        if (this != &other) {
            del(_id);
            _id = other._id;
            other._id = 0;
        }
        return std::move(*this);
    }

    [[nodiscard]] uint id() const { return _id; }
    [[nodiscard]] operator uint() const { return _id; }

    LibObject(const LibObject&) = delete;
    LibObject& operator=(const LibObject&) = delete;

private:
    uint _id;
};

} // namespace Core

} // namespace TotoEngine

#pragma once

#include <AL/al.h>
#include <TotoEngine/Audio/Allocators.hpp>
#include <TotoEngine/Core/Instantiation.hpp>
#include <TotoEngine/Core/LibObject.hpp>

#include <format>
#include <stdexcept>

namespace TotoEngine {

namespace Audio {

using ALBuffer = Core::LibObject<createBuffer, deleteBuffer>;
using ALSource = Core::LibObject<createSource, deleteSource>;

class Sample {
public:
    Sample()
        : _buffer(),
          _source() {
        if (auto error = alGetError()) {
            throw std::runtime_error(std::format("ERROR::SAMPLE ({})", error));
        }
    }

    Sample(ALenum format, const ALvoid* data, ALsizei size, ALsizei sample_rate)
        : Sample() {
        alBufferData(_buffer, format, data, size, sample_rate);
        alSourcei(_source, AL_BUFFER, _buffer);
    }

    void play() const { alSourcePlay(_source); }
    void pause() const { alSourcePause(_source); }
    void stop() const { alSourceStop(_source); }
    void rewind() const { alSourceRewind(_source); }
    ALint state() const { return getProperty(AL_SOURCE_STATE); }

    ALint getProperty(ALenum param) const {
        ALint value;
        alGetSourcei(_source, param, &value);
        return value;
    }

private:
    ALBuffer _buffer;
    ALSource _source;
};

using SampleManager = Core::Manager<Sample>;
using SampleInstance = Core::Manager<Sample>::Instance;

} // namespace Audio

} // namespace TotoEngine

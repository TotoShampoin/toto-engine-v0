#pragma once

#include <AL/al.h>
#include <vector>

namespace TotoEngine {

struct WaveformAudioFile {
    struct Header {
        ALubyte channels;
        ALuint sample_rate;
        ALenum format;
        ALubyte bits_per_sample;
    } header;
    std::vector<char> data;
    size_t size;
};

}

#pragma once

#include <AL/al.h>
#include <vector>
#include <filesystem>

#include <TotoEngine/Audio/Sample.hpp>

namespace TotoEngine {

namespace Audio {

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

WaveformAudioFile loadWavefile(const std::filesystem::path& path);

Sample loadSample(const std::filesystem::path& path);

}

}

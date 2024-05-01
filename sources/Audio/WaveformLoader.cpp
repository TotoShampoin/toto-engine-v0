#include <TotoEngine/Audio/WaveformLoader.hpp>
#include <cassert>
#include <cstring>
#include <format>
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace TotoEngine {

void loadWavefileHeader(std::ifstream& file, WaveformAudioFile& wav) {
    char buffer[5];
    buffer[4] = '\0';

    auto read_n_bytes = [&](int n, const std::string& step = "") {
        assert(n <= 4 && "DON'T LOAD MORE THAN 4 BYTES");
        if(!file.read(buffer, n))
            throw std::runtime_error(std::format("ERROR: invalid WAVE file ({})", step));
        buffer[n] = '\0';
    };
    auto bytes_to_int = [&](std::size_t len) {
        std::int32_t a = 0;
        if(std::endian::native == std::endian::little)
            std::memcpy(&a, buffer, len);
        else
            for(std::size_t i = 0; i < len; ++i)
                reinterpret_cast<char*>(&a)[3 - i] = buffer[i];
        return a;
    };

    if(!file.is_open())
        throw std::runtime_error("Couldn't open the file");
    read_n_bytes(4, "RIFF");
    if(std::strncmp(buffer, "RIFF", 4))
        throw std::runtime_error("ERROR: invalid WAVE file");
    read_n_bytes(4, "File size");
    read_n_bytes(4, "WAVE");
    if(std::strncmp(buffer, "WAVE", 4))
        throw std::runtime_error("ERROR: invalid WAVE file");
    read_n_bytes(4, "Format chunk marker");
    read_n_bytes(4, "FMT length");
    read_n_bytes(2, "Type format (PCM)");
    read_n_bytes(2, "Number of channels");
    wav.header.channels = bytes_to_int(2);
    read_n_bytes(4, "Sample rate");
    wav.header.sample_rate = bytes_to_int(4);
    read_n_bytes(4, "(Sample Rate * BitsPerSample * Channels) / 8");
    read_n_bytes(2, "Format");
    int format = bytes_to_int(2);
    switch (format) {
        case 1:
            wav.header.format = AL_FORMAT_MONO8;
            break;
        case 2:
            wav.header.format = AL_FORMAT_STEREO8;
            break;
        case 3:
            wav.header.format = AL_FORMAT_MONO16;
            break;
        case 4:
            wav.header.format = AL_FORMAT_STEREO16;
            break;
    }
    read_n_bytes(2, "Bits per sample");
    wav.header.bits_per_sample = bytes_to_int(2);
    read_n_bytes(4, "data (header)");
    read_n_bytes(4, "Data size");
    auto size = bytes_to_int(4);
    wav.size = size;
}

WaveformAudioFile loadWavefile(const std::filesystem::path& path) {
    auto file = std::ifstream(path);
    auto wav = WaveformAudioFile();
    loadWavefileHeader(file, wav);

    std::string format_string;
    switch(wav.header.format) {
        case AL_FORMAT_MONO8:
            format_string = "AL_FORMAT_MONO8";
            break;
        case AL_FORMAT_STEREO8:
            format_string = "AL_FORMAT_STEREO8";
            break;
        case AL_FORMAT_MONO16:
            format_string = "AL_FORMAT_MONO16";
            break;
        case AL_FORMAT_STEREO16:
            format_string = "AL_FORMAT_STEREO16";
            break;
    }

    std::cout
        << std::format("Channels    : {}\n", wav.header.channels)
        << std::format("Sample rate : {}\n", wav.header.sample_rate)
        << std::format("Format      : {}\n", format_string)
        << std::format("Bits /sample: {}\n", wav.header.bits_per_sample)
        << std::format("Size        : {}\n", wav.size)
    ;
    wav.data.reserve(wav.size);
    file.read(wav.data.data(), wav.size);

    return wav;
}

}

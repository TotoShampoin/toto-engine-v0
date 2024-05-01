#pragma once

#include <TotoEngine/Audio/FileFormats.hpp>

#include <filesystem>

namespace TotoEngine {

WaveformAudioFile loadWavefile(const std::filesystem::path& path);

}

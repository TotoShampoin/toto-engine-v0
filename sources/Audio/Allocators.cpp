#include <TotoEngine/Audio/Allocators.hpp>

namespace TotoEngine {

namespace Audio {

ALuint createBuffer() {
    ALuint buffer;
    alGenBuffers(1, &buffer);
    return buffer;
}
void deleteBuffer(ALuint& buffer) {
    alDeleteBuffers(1, &buffer);
}
ALuint createSource() {
    ALuint source;
    alGenSources(1, &source);
    return source;
}
void deleteSource(ALuint& source) {
    alDeleteSources(1, &source);
}

} // namespace Audio

} // namespace TotoEngine

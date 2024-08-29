#pragma once

#include <AL/al.h>

namespace TotoEngine {

namespace Audio {

ALuint createBuffer();
void deleteBuffer(ALuint& buffer);
ALuint createSource();
void deleteSource(ALuint& source);

} // namespace Audio

} // namespace TotoEngine
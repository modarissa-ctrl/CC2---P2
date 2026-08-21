#include "WavLoader.h"
#include <fstream>
#include <cstdint>
#include <cstring>
#include <cmath>
#include <sstream>

namespace {
    uint32_t rd32(const unsigned char* p) {
        return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
    }
    uint16_t rd16(const unsigned char* p) {
        return (uint16_t)((uint32_t)p[0] | ((uint32_t)p[1] << 8));
    }
    bool tagIs(const unsigned char* p, const char* tag) {
        return p[0] == (unsigned char)tag[0] && p[1] == (unsigned char)tag[1]
            && p[2] == (unsigned char)tag[2] && p[3] == (unsigned char)tag[3];
    }
}

WavLoader::WavLoader() { reset(); }

void WavLoader::reset() {
    samples.clear();
    error.clear();
    sourceRate = 0;
    sourceChannels = 0;
}

bool WavLoader::load(const std::string& path, float targetRate) {
    reset();

    std::vector<unsigned char> bytes;
    if (!readFile(path, bytes)) return false;
    if (!parse(bytes))          return false;

    resampleTo(targetRate);
    return !samples.empty();
}

bool WavLoader::readFile(const std::string& path, std::vector<unsigned char>& bytes) {
    std::ifstream in(path.c_str(), std::ios::binary);
    if (!in) { error = "cannot open file"; return false; }

    bytes.assign((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    if (bytes.size() < 44) { error = "file is too short to be a WAV"; return false; }
    return true;
}

bool WavLoader::parse(const std::vector<unsigned char>& b) {
    if (!tagIs(&b[0], "RIFF") || !tagIs(&b[8], "WAVE")) {
        error = "not a RIFF/WAVE file";
        return false;
    }

    uint16_t format = 0, channels = 0, bits = 0;
    uint32_t rate = 0;
    size_t dataPos = 0, dataLen = 0;

    // Walk the chunk list rather than assuming fixed offsets. Real files are full of LIST, fact and cue chunks, and their order is not guaranteed.
    size_t pos = 12;
    while (pos + 8 <= b.size()) {
        const unsigned char* id = &b[pos];
        uint32_t len = rd32(&b[pos + 4]);
        size_t   body = pos + 8;

        if (body + len > b.size()) len = (uint32_t)(b.size() - body);   // tolerate truncation

        if (tagIs(id, "fmt ") && len >= 16) {
            format = rd16(&b[body + 0]);
            channels = rd16(&b[body + 2]);
            rate = rd32(&b[body + 4]);
            bits = rd16(&b[body + 14]);
        }
        else if (tagIs(id, "data")) {
            dataPos = body;
            dataLen = len;
        }

        pos = body + len + (len & 1);   // chunks are word aligned
    }

    if (channels == 0 || dataLen == 0) { error = "no usable fmt or data chunk"; return false; }

    // 1 is PCM, 3 is IEEE float, 0xFFFE is WAVE_FORMAT_EXTENSIBLE
    const bool pcm16 = (format == 1 || format == 0xFFFE) && bits == 16;
    const bool flt32 = (format == 3 || format == 0xFFFE) && bits == 32;
    if (!pcm16 && !flt32) {
        std::ostringstream m;
        m << "format " << format << " at " << bits
            << " bits is not supported, re-export as 16 bit PCM WAV";
        error = m.str();
        return false;
    }

    const size_t bytesPerSample = bits / 8;
    const size_t frames = dataLen / (bytesPerSample * channels);

    // Downmix to mono. The caller reads one sample per call, so a stereo buffer left interleaved would play at half speed and an octave down.
    samples.reserve(frames);
    for (size_t f = 0; f < frames; ++f) {
        float acc = 0.0f;
        for (size_t c = 0; c < channels; ++c) {
            const unsigned char* p = &b[dataPos + (f * channels + c) * bytesPerSample];
            if (pcm16) {
                acc += (float)(int16_t)rd16(p) / 32768.0f;
            }
            else {
                uint32_t raw = rd32(p);
                float v;
                std::memcpy(&v, &raw, sizeof(v));
                acc += v;
            }
        }
        samples.push_back(acc / (float)channels);
    }

    sourceRate = (int)rate;
    sourceChannels = (int)channels;

    if (samples.empty()) { error = "data chunk contained no frames"; return false; }
    return true;
}

void WavLoader::resampleTo(float targetRate) {
    if (sourceRate <= 0 || targetRate <= 0.0f) return;
    if (std::fabs((float)sourceRate - targetRate) <= 1.0f) return;

    // Linear interpolation. Good enough for short one shots, and it avoids a 48000 clip coming out sharp against a 44100 stream.
    const float step = (float)sourceRate / targetRate;

    std::vector<float> out;
    out.reserve((size_t)((float)samples.size() / step) + 1);

    for (float p = 0.0f; p + 1.0f < (float)samples.size(); p += step) {
        size_t i = (size_t)p;
        float  frac = p - (float)i;
        out.push_back(samples[i] * (1.0f - frac) + samples[i + 1] * frac);
    }

    samples.swap(out);
}

#include "NoiseVoice.h"

NoiseVoice::NoiseVoice() {
    lastOut = 0.0f;
    rngState = 2463534242u;
}

// Custom random number generator rather than rand()
float NoiseVoice::whiteNoise() {
    rngState = rngState * 1664525u + 1013904223u;
    return ((float)(rngState >> 8) / 8388608.0f) - 1.0f;   // -1 .. 1
}

float NoiseVoice::waveform(float phase) {
    (void)phase;   // noise has no cycle, so the phase is not used

    // A one pole low pass, with the note frequency setting the cutoff. Without this it is just static and every key sounds identical.
    float k = frequency / (sampleRate * 0.5f);
    if (k > 1.0f)   k = 1.0f;
    if (k < 0.001f) k = 0.001f;

    lastOut += k * (whiteNoise() - lastOut);
    return lastOut * 2.0f;   
}

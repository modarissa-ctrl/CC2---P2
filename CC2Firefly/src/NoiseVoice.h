#pragma once
#include "OscillatorVoice.h"

class NoiseVoice : public OscillatorVoice {
public:
    NoiseVoice();
protected:
    float waveform(float phase);
private:
    float lastOut;
    unsigned int rngState;
    float whiteNoise();
};

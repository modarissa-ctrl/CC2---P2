#pragma once
#include "OscillatorVoice.h"

class SineVoice : public OscillatorVoice {
protected:
    float waveform(float phase);
};

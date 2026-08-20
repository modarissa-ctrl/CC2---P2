#pragma once
#include "OscillatorVoice.h"

class PulseVoice : public OscillatorVoice {
public:
    PulseVoice();
    void setPulseWidth(float w);  
protected:
    float waveform(float phase);
private:
    float pulseWidth;
};

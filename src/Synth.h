#pragma once
#include <vector>

// Synth.h - Ronald owns the implementation, I just use this interface
// renamed from proposed_Synth.h - only change is adding std:: to vector
// because it wouldn't compile without it

class Synth {
public:
    void noteOn(int voiceSlot, float freq);
    void noteOff(int voiceSlot);
    void setFilterCutoff(float value);
    void setMasterVolume(float value);
    void copyScope(std::vector<float>& out);
};

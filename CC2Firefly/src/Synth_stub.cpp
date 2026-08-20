#include "Synth.h"

// this is just a placeholder so my code compiles without Ronald's engine
// when he sends his real Synth.cpp this file gets deleted and replaced

void Synth::noteOn(int voiceSlot, float freq) {
    // stub - does nothing until Ronald connects the real engine
}

void Synth::noteOff(int voiceSlot) {
    // stub
}

void Synth::setFilterCutoff(float value) {
    // stub
}

void Synth::setMasterVolume(float value) {
    // stub
}

void Synth::copyScope(std::vector<float>& out) {
    // returns a flat buffer so Visualiser draws a flat line for now
    out.assign(512, 0.0);
}

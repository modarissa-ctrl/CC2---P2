#include "PulseVoice.h"

PulseVoice::PulseVoice() { pulseWidth = 0.5f; }

void PulseVoice::setPulseWidth(float w) {
    if (w < 0.05f) w = 0.05f;
    if (w > 0.95f) w = 0.95f;
    pulseWidth = w;
}

float PulseVoice::waveform(float phase) {
    // Half amplitude deliberately since a square wave carries a lot more energy than a sine at the same peak level, so at 1.0 it drowns out the other voices.
    return (phase < pulseWidth) ? 0.5f : -0.5f;
}

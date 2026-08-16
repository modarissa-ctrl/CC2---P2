#pragma once
#include <vector>

// Synth.h - Ronald owns the implementation, Modar uses this interface
// These are the 5 methods I need to call from InputMapper and Visualiser.
// Ronald: please add/remove/rename anything here, just let me know what changes
// so I can update my side before we both get too deep into coding.

class Synth {
public:

    // called by InputMapper when a key is pressed
    // voiceSlot = which voice in the pool to use (0 to N-1)
    // freq = frequency in Hz (e.g. 440.0 for A4)
    void noteOn(int voiceSlot, float freq);

    // called by InputMapper when a key is released
    void noteOff(int voiceSlot);

    // called by InputMapper when the mouse moves
    // value is normalised 0.0 to 1.0
    void setFilterCutoff(float value);
    void setMasterVolume(float value);

    // called by Visualiser each frame to get a snapshot of the audio buffer
    // please use try_lock inside so the audio thread never waits on my drawing code
    void copyScope(vector<float>& out);
};

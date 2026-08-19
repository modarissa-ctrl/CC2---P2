#pragma once
#include "ofMain.h"
#include "SoundSource.h"
#include "Envelope.h"
#include <vector>
#include <string>

using namespace std;

// SampleVoice plays a pre-recorded audio clip.
// Inherits from SoundSource (inheritance relationship).
// Owns its own Envelope (composition relationship).
// Synth owns the SampleVoice objects - same as it owns SineVoice etc.
class SampleVoice : public SoundSource {
public:
    SampleVoice();

    // load the clip into memory - call this in setup(), not during audio playback
    bool load(string filename);

    // SoundSource interface
    float nextSample();
    void noteOn(float freq);  // freq is ignored for clips, we just reset the playhead
    void noteOff();
    bool isActive();

private:
    vector<float> samples;   // the clip data, loaded at startup
    int playHead;
    bool playing;

    Envelope envelope;  // owned by this voice (composition)
};

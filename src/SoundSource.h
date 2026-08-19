#pragma once

// SoundSource.h - shared base class (renamed from proposed_SoundSource.h).
// Ronald implements SineVoice, PulseVoice, NoiseVoice inheriting from this.
// Modar implements SampleVoice inheriting from this.

class SoundSource {
public:
    virtual float nextSample() = 0;
    virtual void noteOn(float freq) = 0;
    virtual void noteOff() = 0;
    virtual bool isActive() = 0;

    virtual ~SoundSource() {}
};

#pragma once

// SoundSource.h - shared base class
// Ronald: this is the proposed interface. Every voice type (Sine, Pulse, Noise, Sample)
// inherits from this so Synth can treat them all the same.
// Please confirm before we both start building on top of it.

class SoundSource {
public:
    virtual float nextSample() = 0;
    virtual void noteOn(float freq) = 0;
    virtual void noteOff() = 0;
    virtual bool isActive() = 0;

    virtual ~SoundSource() {}
};

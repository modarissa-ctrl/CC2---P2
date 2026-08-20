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

	// Added this since Synth needs to set the sample rate for voices that need it (e.g., SampleVoice).
	// Not purely virtual since it has a default body but it can be overridden by derived classes that need to do something with the sample rate.
	virtual void setSampleRate(float sampleRate) { (void)sampleRate; }

    virtual ~SoundSource() {}

};

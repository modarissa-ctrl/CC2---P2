#pragma once
#include "SoundSource.h"
#include "Envelope.h"

// OscillatorVoice.h - base class for all generated oscillator voices (Sine, Pulse, Noise, Sample) i.e. non-clip based voices.
// Inherits from SoundSource and implements the common functionality for all oscillator voices, including the envelope handling.
// Relationships showcased: inheritance (OscillatorVoice inherits from SoundSource) and composition (OscillatorVoice has an Envelope)

class OscillatorVoice : public SoundSource {
public:
	OscillatorVoice();
	virtual ~OscillatorVoice() {}

	float nextSample();
	void noteOn(float freq);
	void noteOff();
	bool isActive();
	void setSampleRate(float sr);

	Envelope & getEnvelope() { return envelope; }  // Provide access to the envelope for setting ADSR parameters


protected:	
	Envelope envelope;  // Each OscillatorVoice has an Envelope to manage its amplitude over time
	float frequency;     // Frequency of the oscillator in Hz
	float phase;         // Current phase of the oscillator (0.0 to 1.0)
	float phaseStep;     // Amount to increment the phase each sample
	float sampleRate;    // Sample rate for the oscillator

	virtual float waveform(float phase) = 0;  // Derived classes will implement their own waveform generation based on the current phase
};
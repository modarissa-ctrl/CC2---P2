#include "OscillatorVoice.h"

OscillatorVoice::OscillatorVoice() {
	phase = 0.0f;
	phaseStep = 0.0f;
	frequency = 440.0f; // default frequency (A4)
	sampleRate = 44100.0f; // default sample rate

	envelope.setSampleRate(sampleRate);  // Ensure the envelope has the correct sample rate
}

void OscillatorVoice::setSampleRate(float sr) {
	if (sr <= 0.0f) {
		sr = 44100.0f; // Default to 44100 Hz if an invalid sample rate is provided
	}

	sampleRate = sr;
	envelope.setSampleRate(sampleRate);  // Update the envelope's sample rate
	phaseStep = frequency / sampleRate; // Update phase step based on new sample rate
}

void OscillatorVoice::noteOn(float freq) {
	if (freq > 0.0f) {
		frequency = freq;
	}
	else {
		frequency = 440.0f; // Default to A4 if an invalid frequency is provided
	}

	phaseStep = frequency / sampleRate; // Update phase step based on new frequency
	phase = 0.0f; // Reset phase to start the waveform from the beginning
	envelope.noteOn();
}

void OscillatorVoice::noteOff() {
	envelope.noteOff();
}

bool OscillatorVoice::isActive() {
	// Check if the envelope is still active (i.e., not finished)
	return envelope.isActive();
}

float OscillatorVoice::nextSample() {
	if (!envelope.isActive()) {
		return 0.0f; // Return silence if the envelope is not active
	}

	float s = waveform(phase) * envelope.nextSample(); // Generate the waveform sample and apply the envelope

	phase += phaseStep; // Increment the phase for the next sample
	if (phase >= 1.0f) {
		phase -= 1.0f; // Wrap the phase to stay within [0, 1)
	}

	return s;
}
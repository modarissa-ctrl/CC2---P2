#include "Envelope.h"

Envelope::Envelope() {
	stage = IDLE;
	level = 0.0f;
	sampleRate = 44100.0f; // default sample rate
	attackTime = 0.01f;    // default attack time in seconds - 10ms is a common default since it feels instantaneous to the human ear
	decayTime = 0.1f;      // default decay time in seconds - play around with this
	sustainLevel = 0.7f;   // default sustain level (0.0 to 1.0) - play around with this
	releaseTime = 0.2f;    // default release time in seconds - play around with this
	recalcRates();
}

void Envelope::setSampleRate(float sr) {
	if (sr > 0.0f) {
		this->sampleRate = sr;
		recalcRates();
	}
}

void Envelope::setADSR(float attack, float decay, float sustain, float release) {
	attackTime = attack;
	decayTime = decay;
	sustainLevel = sustain;
	releaseTime = release;
	recalcRates();
}

// Recalculate the attack, decay, and release rates based on the current sample rate and ADSR parameters
// Implemented here so nextSample() has no computational overhead for calculating the rates on each sample. The rates are calculated once when the ADSR parameters or sample rate change.
void Envelope::recalcRates() {
	attackRate = (attackTime > 0.0f) ? (1.0f / (attackTime * sampleRate)) : 1.0f;
	decayRate = (decayTime > 0.0f) ? ((1.0f - sustainLevel) / (decayTime * sampleRate)) : 1.0f;
	releaseRate = (releaseTime > 0.0f) ? (sustainLevel / (releaseTime * sampleRate)) : 1.0f;
}

void Envelope::noteOn() {
	stage = ATTACK;
}

void Envelope::noteOff() {
	if (stage != IDLE) {  // Only transition to RELEASE if we're not already in the IDLE stage
		stage = RELEASE;
	}
}

bool Envelope::isActive() const {
	return stage != IDLE;
}

float Envelope::nextSample() {
	switch (stage) {
	case IDLE:
		return 0.0f;
	case ATTACK:
		level += attackRate;
		if (level >= 1.0f) {
			level = 1.0f;
			stage = DECAY;
		}
		break;
	case DECAY:
		level -= decayRate;
		if (level <= sustainLevel) {
			level = sustainLevel;
			stage = SUSTAIN;
		}
		break;
	case SUSTAIN:
		// Sustain level is maintained until noteOff() is called
		level = sustainLevel;
		break;
	case RELEASE:
		level -= releaseRate;
		if (level <= 0.0f) {
			level = 0.0f;
			stage = IDLE; // Transition to IDLE when the release phase is complete
		}
		break;
	default:
		break;
	}
	return level;
}
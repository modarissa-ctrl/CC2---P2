#include "Synth.h"
#include <algorithm>

Synth::Synth() {
	currentType = 0;
	sampleRate = 44100.0f;
	masterVolume = 0.5f;
	cutoff01 = 1.0f;
	filterState = 0.0f;
	isReady = false;
	for (int i = 0; i < MELODIC_SLOTS; ++i) {
		slotType[i] = 0;
	}
}

void Synth::setup(size_t sr, size_t bufferSize) {
	sampleRate = (float)sr;

	// Create voices for each waveform type HERE. After this, creation and destruction of voices is not allowed. This is because the voices are used in the render() method 
	// which is called from a different thread than this one. If we were to create or destroy voices while render() was running, it would cause a crash.
	sineVoices.resize(MELODIC_SLOTS);
	pulseVoices.resize(MELODIC_SLOTS);
	noiseVoices.resize(MELODIC_SLOTS);

	mono.assign(bufferSize, 0.0f);
	scope.assign(bufferSize, 0.0f);

	// Pointers are taken AFTER the resizing of the vectors above, because resizing invalidates pointers. This is why we don't just create the voices in the constructor, because we don't know the buffer size yet.
	buildVoiceList();

	for (size_t i = 0; i < allVoices.size(); ++i) {
		allVoices[i]->setSampleRate(sampleRate);
	}
	
	isReady = true;
}
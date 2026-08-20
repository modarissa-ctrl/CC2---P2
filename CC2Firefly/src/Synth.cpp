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
	clipVoices.resize(CLIP_SLOTS);

	mono.assign(bufferSize, 0.0f);
	scope.assign(bufferSize, 0.0f);

	// Pointers are taken AFTER the resizing of the vectors above, because resizing invalidates pointers. This is why we don't just create the voices in the constructor, because we don't know the buffer size yet.
	buildVoiceList();

	for (size_t i = 0; i < allVoices.size(); ++i) {
		allVoices[i]->setSampleRate(sampleRate);
	}
	
	isReady = true;
}

void Synth::buildVoiceList() {
	allVoices.clear();
	allVoices.reserve(MELODIC_SLOTS * NUM_TYPES + CLIP_SLOTS);
	for (int i = 0; i < MELODIC_SLOTS; ++i) {
		allVoices.push_back(&sineVoices[i]);
		allVoices.push_back(&pulseVoices[i]);
		allVoices.push_back(&noiseVoices[i]);
	}

	for (int i = 0; i < CLIP_SLOTS; ++i) {
		allVoices.push_back(&clipVoices[i]);
	}
}

SoundSource* Synth::melodicVoice(int type, int slot) {
	if (slot < 0 || slot >= MELODIC_SLOTS) return nullptr;

	switch (type) {
	case 0: return &sineVoices[slot];
	case 1: return &pulseVoices[slot];
	case 2: return &noiseVoices[slot];
	default: return &sineVoices[slot];
	}
}

// Modar's original interface methods but fully fleshed out
void Synth::noteOn(int voiceSlot, float freq) {
	if (!isReady) return;

	if (voiceSlot < 0 || voiceSlot >= VOICE_POOL_SIZE) return;

	if (voiceSlot >= MELODIC_SLOTS) {
		int clipIndex = voiceSlot - MELODIC_SLOTS;
		clipVoices[clipIndex].noteOn(freq); 
		return;
	}
	
	slotType[voiceSlot] = currentType;
	SoundSource* voice = melodicVoice(currentType, voiceSlot);
	if (voice) {
		voice->noteOn(freq);
	}
}

void Synth::noteOff(int voiceSlot) {
	if (!isReady) return;

	if (voiceSlot < 0 || voiceSlot >= VOICE_POOL_SIZE) return;

	if (voiceSlot >= MELODIC_SLOTS) {
		int clipIndex = voiceSlot - MELODIC_SLOTS;
		clipVoices[clipIndex].noteOff();
		return;
	}

	// Released against the type it was STARTED with, not the current type lest it continues playing indefinitely.
	SoundSource* voice = melodicVoice(slotType[voiceSlot], voiceSlot);
	if (voice) voice->noteOff();
}

void Synth::setFilterCutoff(float value) {
	if (value < 0.0f) value = 0.0f;
	if (value > 1.0f) value = 1.0f;
	cutoff01 = value;
}

void Synth::setMasterVolume(float value) {
	if (value < 0.0f) value = 0.0f;
	if (value > 1.0f) value = 1.0f;
	masterVolume = value;
}

// Called from Visualiser::updte() on main thread
void Synth::copyScope(std::vector<float>& out) {
	std::unique_lock<std::mutex> lock(scopeMutex, std::try_to_lock);
	
	if (!lock.owns_lock()) return; // If we can't get the lock, just skip this frame

	out = scope;
}


// Additional methods by Ronald

void Synth::setVoiceType(int type) {
	if (type < 0 || type >= NUM_TYPES) return;
	currentType = type;
}

int Synth::getVoiceType() const { return currentType; }

std::string Synth::getVoiceTypeName() const {
	switch (currentType) {
	case 1:  return "PULSE";
	case 2:  return "NOISE";
	default: return "SINE";
	}
}

bool Synth::loadClip(int clipIndex, const std::string& filename) {
	if (clipIndex < 0 || clipIndex >= CLIP_SLOTS) return false;
	return clipVoices[clipIndex].load(filename, sampleRate);
}

void Synth::render(ofSoundBuffer& out) {
	const size_t nFrames = out.getNumFrames();
	const size_t nChans = out.getNumChannels();
	if (!isReady || nChans == 0) return;

	// If sound card ever hands us a bigger buffer than we sized for, render what fits rather than resizing on this thread.
	const size_t n = (nFrames < mono.size()) ? nFrames : mono.size();

	// Squaring makes the mouse sweep feel even to the ear instead of doing everything in the last centimetre of travel.
	float k = cutoff01 * cutoff01;
	if (k < 0.0015f) k = 0.0015f;
	if (k > 1.0f)    k = 1.0f;

	for (size_t f = 0; f < n; ++f) {
		float mix = 0.0f;

		for (size_t v = 0; v < allVoices.size(); ++v) {
			SoundSource* voice = allVoices[v];
			if (voice->isActive()) mix += voice->nextSample();   // polymorphic
		}

		filterState += k * (mix - filterState);

		float s = filterState * masterVolume;
		if (s > 1.0f) s = 1.0f;
		if (s < -1.0f) s = -1.0f;

		mono[f] = s;
		for (size_t c = 0; c < nChans; ++c) out[f * nChans + c] = s;
	}

	std::unique_lock<std::mutex> lock(scopeMutex, std::try_to_lock);
	if (lock.owns_lock()) {
		std::copy(mono.begin(), mono.begin() + n, scope.begin());
	}
}

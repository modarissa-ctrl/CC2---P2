#pragma once

#include "SoundSource.h"
#include "SineVoice.h"
#include "PulseVoice.h"
#include "NoiseVoice.h"

#include <vector>
#include <string>
#include <mutex>

// Synth.h - Ronald owns the implementation, I just use this interface
// renamed from proposed_Synth.h - only change is adding std:: to vector
// because it wouldn't compile without it

// Modar - I've left the five methods you initalised this file with unchanged (so InputMapper.cpp and Visualiser.cpp won't break as a result of my changes)

class Synth {
public:
    Synth();

	// Modar's original methods
    void noteOn(int voiceSlot, float freq);
    void noteOff(int voiceSlot);
    void setFilterCutoff(float value);
    void setMasterVolume(float value);
    void copyScope(std::vector<float>& out);
	// end of Modar's original methods

	void setup(size_t sampleRate, size_t bufferSize);
	void render(ofSoundBuffer& out);                // Called from ofApp::audioOut()

    // Which of the three sounds the keys play
	void setVoiceType(int voiceType);
	int getVoiceType() const;
	std::string getVoiceTypeName() const;

	bool loadClip(int clipIndex, const std::string& filePath);

	static const int MELODIC_SLOTS = 5;		// Slots 0 - 4
	static const int CLIP_SLOTS = 3;		// Slots 5 - 7
	static const int VOICE_POOL_SIZE = 8;		// Matches InputMapper
	static const int NUM_TYPES = 3;

private:
	// One set of voices per waveform, slot picks the index while the current type picks the set
	// Noting is created or destroyed once sound card is running
	std::vector<SineVoice> sineVoices;
	std::vector<PulseVoice> pulseVoices;
	std::vector<NoiseVoice> noiseVoices;
	
	// List of pointers to every voice above. Showcases Polymorphism via render() which uses this list but never knows which actual class it is talking to
	std::vector<SoundSource*> allVoices;

	// Current waveform
	int slotType[MELODIC_SLOTS];

	int currentType;

	std::vector<float> mono;
	std::vector<float> scope;
	std::mutex scopeMutex;

	float sampleRate;
	float masterVolume;
	float cutoff01;
	float filterState;

	bool isReady;

	SoundSource* melodicVoice(int type, int slot);
	void buildVoiceList();

};

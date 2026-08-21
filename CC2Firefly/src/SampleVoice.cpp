#include "SampleVoice.h"
#include "WavLoader.h"
#include <cmath>

SampleVoice::SampleVoice() {
    this->playHead = 0;
    this->playing = false;
}

bool SampleVoice::load(string filename, float targetRate) {
    // Buggy ofSoundFile based code commented out below, entirely new implementation based on custom Wav loader
    /*
    // load the audio file before the app starts playing anything
    ofSoundFile file;

    if (!file.load(ofToDataPath(filename))) {
        cout << "SampleVoice: couldn't load file: " << filename << endl;
        return false;
    }

    ofSoundBuffer buf;
    file.readTo(buf);

    // copy the samples out into our own vector
    // not 100% sure if getBuffer() is the right call here - check if it doesn't compile

    // Modar - buf.getBuffer only works properly for mono sound. Modified to make it work for stereo  as well
    // samples = buf.getBuffer();
    ofSoundBuffer mono;
	if (buf.getNumChannels() > 1) {
		buf.getChannel(mono, 0); // left channel
	}
	else {
		mono = buf; // already mono
	}

	float fileRate = mono.getSampleRate();
    if (fileRate > 0.0f && fabs(fileRate - targetRate) > 1.0f) {
		mono.resample(targetRate);
	}

    samples = mono.getBuffer();

	envelope.setSampleRate(targetRate);   //
	envelope.setADSR(0.002f, 0.010f, 1.0f, 0.030f);

    cout << "SampleVoice: loaded " << filename << " (" << samples.size() << " samples)" << endl;

    return samples.size() > 0;
    */

    // Read the file up front, in setup(). Never on the audio thread.
    WavLoader loader;

    if (!loader.load(ofToDataPath(filename), targetRate)) {
        ofLogError() << "SampleVoice: " << filename << ": " << loader.getError();
        return false;
    }

    samples = loader.getSamples();

    envelope.setSampleRate(targetRate);

    envelope.setADSR(0.002f, 0.010f, 1.0f, 0.030f);

    ofLogNotice() << "SampleVoice: loaded " << filename
        << " (" << samples.size() << " samples, source "
        << loader.getSourceSampleRate() << "Hz "
        << loader.getSourceChannels() << "ch)";

    return !samples.empty();
}

float SampleVoice::nextSample() {
    if (!playing) return 0.0f;

    if (playHead >= (int)samples.size()) {
        playing = false;
		envelope.noteOff();
        return 0.0f;
    }

    float s = samples[playHead] * envelope.nextSample();
    playHead++;
    return s;
}

void SampleVoice::noteOn(float freq) {
    (void)freq;     // freq doesn't do anything for clips, we just need it to match SoundSource
    playHead = 0;
    playing = true;
    envelope.noteOn();
}

void SampleVoice::noteOff() {
    // clips finish on their own so this might not get called much
    envelope.noteOff();
}

bool SampleVoice::isActive() {
    return playing;
}

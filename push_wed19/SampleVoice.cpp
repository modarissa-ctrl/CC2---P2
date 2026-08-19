#include "SampleVoice.h"

SampleVoice::SampleVoice() {
    this->playHead = 0;
    this->playing = false;
}

bool SampleVoice::load(string filename) {
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
    samples = buf.getBuffer();

    cout << "SampleVoice: loaded " << filename << " (" << samples.size() << " samples)" << endl;

    return samples.size() > 0;
}

float SampleVoice::nextSample() {
    if (!playing) return 0.0;

    if (playHead >= samples.size()) {
        playing = false;
        return 0.0;
    }

    float s = samples[playHead] * envelope.nextSample();
    playHead++;
    return s;
}

void SampleVoice::noteOn(float freq) {
    // freq doesn't do anything for clips, we just need it to match SoundSource
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

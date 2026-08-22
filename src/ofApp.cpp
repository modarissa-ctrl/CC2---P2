#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetLogLevel(OF_LOG_WARNING); // Fix: must be first — prevents null logger crash on VS2026 + OF 0.12.1
	ofBackground(20);
	ofSetWindowTitle("CC2 Project 2 - Team Firefly");
	ofSetFrameRate(60);

	sampleRate = 48000;
	bufferSize = 512;

	// Synth allocates every voice and every buffer here
	synth.setup(sampleRate, bufferSize);

	clipsLoaded = 0;
	if (synth.loadClip(0, "clip1.wav")) clipsLoaded++;
	if (synth.loadClip(1, "clip2.wav")) clipsLoaded++;
	if (synth.loadClip(2, "clip3.wav")) clipsLoaded++;
	cout << "clips loaded: " << clipsLoaded << " of 3";

	inputMapper = new InputMapper(&synth);
	visualiser = new Visualiser(&synth);

	//phase = 0.0f;
	//phaseStep = 440.0f / 44100.0f;	// 440 Hz tone at 44100 samples/sec (fraction of a cycle per sample)

	ofSoundStreamSettings settings;
	settings.setOutListener(this); // set the audioOut() method to be called when audio is needed
	settings.sampleRate = 48000;
	settings.numOutputChannels = 2; // stereo
	settings.numInputChannels = 0; // no audio input
	settings.bufferSize = 512;
	settings.numBuffers = 4;

	// soundStream.printDeviceList();  // Fix: crashes on VS2026 — null logger channel before RtAudio initialises
	settings.setApi(ofSoundDevice::Api::MS_WASAPI);
	soundStream.setup(settings);
}

//--------------------------------------------------------------
void ofApp::update() {
	visualiser->update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	//ofDrawBitmapStringHighlight("Press any key to stop the sound (if any!)", 20, 20);
	visualiser->draw(0, 0, ofGetWidth(), ofGetHeight());
	visualiser->drawInfo(inputMapper->getActiveNoteNames(), 12, ofGetHeight() - 78);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	inputMapper->keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	inputMapper->keyReleased(key); // Fix: was calling keyPressed — notes would never release
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
	inputMapper->mouseMoved(x, y, ofGetWidth(), ofGetHeight());
}

void ofApp::exit() {
	// Important: stop the audio thread first
	soundStream.close();

	delete inputMapper;
	delete visualiser;
	inputMapper = NULL;
	visualiser = NULL;
}

// Remember that audioOut() is called on a separate thread, so it should be as efficient as possible and not call any OpenFrameworks drawing functions or anything else that might take a long time to execute.
// It is also important to avoid using any variables that are being modified in the main thread (e.g. in update() or keyPressed()) without some kind of mutex lock, because the main thread and the audio thread can access those variables at the same time, which can cause crashes or other problems.
//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer & buffer) {
	// The commented out code below was to test audio generation. It is not needed for the final project, but it is left here for reference in case we want to experiment with generating audio in the future.

	/*
	size_t numFrames = buffer.getNumFrames();
	size_t numChannels = buffer.getNumChannels();


	for (size_t i = 0; i < numFrames; i++) {
		float sample = sinf(phase * TWO_PI);	// sine wave in range -1 to 1

		buffer[i * numChannels] = sample;			// left channel
		buffer[i * numChannels + 1] = sample;	// right channel
		
		phase += phaseStep;
		if (phase >= 1.0f) {
			phase -= 1.0f;	// wrap phase back to 0 when it reaches 1
		}
	}
	*/
	// end of test audio generation code

	synth.render(buffer);
}

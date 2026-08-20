#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(20);
	ofSetWindowTitle("CC2 Project 2 - Team Firefly");
	ofSetFrameRate(60);

	sampleRate = 44100;
	bufferSize = 512;

	// Synth allocates every voice and every buffer here
	synth.setup(sampleRate, bufferSize);
	
	//phase = 0.0f;
	//phaseStep = 440.0f / 44100.0f;	// 440 Hz tone at 44100 samples/sec (fraction of a cycle per sample)

	ofSoundStreamSettings settings;
	settings.setOutListener(this);	// set the audioOut() method to be called when audio is needed		
	settings.sampleRate = 44100;
	settings.numOutputChannels = 2;	// stereo
	settings.numInputChannels = 0;		// no audio input
	settings.bufferSize = 512;
	settings.numBuffers = 4;

	soundStream.printDeviceList();
	soundStream.setup(settings);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofDrawBitmapStringHighlight("Press any key to stop the sound (if any!)", 20, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

// Remember that audioOut() is called on a separate thread, so it should be as efficient as possible and not call any OpenFrameworks drawing functions or anything else that might take a long time to execute.  
// It is also important to avoid using any variables that are being modified in the main thread (e.g. in update() or keyPressed()) without some kind of mutex lock, because the main thread and the audio thread can access those variables at the same time, which can cause crashes or other problems.
//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer& buffer) {
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

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetLogLevel(OF_LOG_WARNING);  // fix: prevents null logger crash on VS 2026

    ofSetWindowTitle("Team Firefly — SoftSynth");
    ofSetFrameRate(60);
    ofBackground(20, 20, 20);

    // audio stream setup
    ofSoundStreamSettings settings;
    settings.setOutListener(this);
    settings.sampleRate      = 44100;
    settings.numOutputChannels = 2;
    settings.numInputChannels  = 0;
    settings.bufferSize      = 512;
    soundStream.setup(settings);
}

//--------------------------------------------------------------
void ofApp::update() {
    inputMapper.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
    visualiser.draw(
        inputMapper.getActiveNoteNames(),
        synth.getMasterVolume()
    );
}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer& buffer) {
    synth.audioOut(buffer);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    inputMapper.keyPressed(key, synth);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
    inputMapper.keyReleased(key, synth);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
    inputMapper.mouseMoved(x, y, synth);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) { }
void ofApp::mousePressed(int x, int y, int button) { }
void ofApp::mouseReleased(int x, int y, int button) { }
void ofApp::mouseEntered(int x, int y) { }
void ofApp::mouseExited(int x, int y) { }
void ofApp::windowResized(int w, int h) { }
void ofApp::gotMessage(ofMessage msg) { }
void ofApp::dragEvent(ofDragInfo dragInfo) { }

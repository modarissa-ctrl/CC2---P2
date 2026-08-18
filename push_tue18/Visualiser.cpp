#include "Visualiser.h"

Visualiser::Visualiser(Synth* s) {
    this->synth = s;
    scopeBuffer.resize(512, 0.0);
}

void Visualiser::update() {
    // grab the latest audio buffer from the synth
    // Ronald handles the thread safety on his side so this should be fine
    synth->copyScope(scopeBuffer);
}

void Visualiser::draw(int x, int y, int w, int h) {
    // full area for waveform for now, leaving 40px at bottom for key guide
    // TODO: split into waveform + spectrum Thursday
    drawWaveform(x, y, w, h - 40);
    drawKeyGuide(x, y + h - 40);
}

void Visualiser::drawInfo(vector<string> activeNotes, int x, int y) {
    ofSetColor(220, 220, 100);

    string noteStr = "Playing: ";

    if (activeNotes.size() == 0) {
        noteStr += "(none)";
    } else {
        for (int i = 0; i < activeNotes.size(); i++) {
            if (i > 0) noteStr += " + ";
            noteStr += activeNotes[i];
        }
    }

    ofDrawBitmapString(noteStr, x, y);
}

void Visualiser::drawWaveform(int x, int y, int w, int h) {
    // dark background
    ofSetColor(8, 25, 8);
    ofDrawRectangle(x, y, w, h);

    // centre line so you can see when sound is at zero
    ofSetColor(30, 70, 30);
    ofDrawLine(x, y + h / 2, x + w, y + h / 2);

    ofSetColor(60, 120, 60);
    ofDrawBitmapString("WAVEFORM", x + 8, y + 16);

    if (scopeBuffer.size() == 0) return;

    // draw the waveform line
    ofSetColor(0, 230, 70);
    ofNoFill();
    ofBeginShape();
    for (int i = 0; i < scopeBuffer.size(); i++) {
        float px = ofMap(i, 0, scopeBuffer.size() - 1, x + 1, x + w - 1);
        float py = ofMap(scopeBuffer[i], -1.0, 1.0, y + h - 5, y + 5);
        ofVertex(px, py);
    }
    ofEndShape(false);
    ofFill();
}

void Visualiser::drawSpectrum(int x, int y, int w, int h) {
    // TODO: Thursday (push_thu20)
}

void Visualiser::drawKeyGuide(int x, int y) {
    ofSetColor(140, 140, 140);
    ofDrawBitmapString("z=C  s=C#  x=D  d=D#  c=E  v=F  g=F#  b=G  h=G#  n=A  j=A#  m=B  ,=C5", x + 8, y + 14);
    ofDrawBitmapString("Mouse: left/right = filter  |  top/bottom = volume  |  1 2 3 = clips", x + 8, y + 28);
}

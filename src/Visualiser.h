#pragma once
#include "ofMain.h"
#include "Synth.h"
#include <vector>
#include <string>

using namespace std;

// Visualiser - draws the waveform scope and (later) the frequency spectrum.
// It reads from Synth via copyScope() but doesn't own Synth - association.
// ofApp owns Visualiser (composition).
class Visualiser {
public:
    Visualiser(Synth* synth);

    void update();   // grab latest scope data from Synth (non-blocking)
    void draw(int x, int y, int w, int h);

    // call this before draw() so the note names show up on screen
    void drawInfo(vector<string> activeNotes, int x, int y);

private:
    Synth* synth;
    vector<float> scopeBuffer;

    void drawWaveform(int x, int y, int w, int h);
    void drawSpectrum(int x, int y, int w, int h);  // TODO: add Thursday
    void drawKeyGuide(int x, int y);
};

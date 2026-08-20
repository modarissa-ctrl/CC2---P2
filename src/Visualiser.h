#pragma once
#include "ofMain.h"
#include "Synth.h"

class Visualiser {
public:
    Visualiser(Synth* synth);

    void draw(std::vector<std::string> activeNotes, float volume);

private:
    Synth* synth;

    std::vector<float> scopeBuffer;

    static const int NUM_BINS = 256;

    void drawScope();
    void drawSpectrum();
    void drawVolumeBar(float volume);
    void drawInfo(std::vector<std::string> activeNotes);
    void drawKeyGuide();
};

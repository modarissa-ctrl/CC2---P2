#include "Visualiser.h"

Visualiser::Visualiser(Synth* synth) {
    this->synth = synth;
    scopeBuffer.resize(512, 0.0f);
}

void Visualiser::draw(std::vector<std::string> activeNotes, float volume) {
    ofBackground(20, 20, 20);

    drawScope();
    drawSpectrum();
    drawVolumeBar(volume);
    drawInfo(activeNotes);
    drawKeyGuide();
}

// --- Waveform scope (top portion of screen) ---
void Visualiser::drawScope() {
    float w = ofGetWidth();
    float h = ofGetHeight();

    // scope lives in the top 55% of the screen
    float scopeTop    = 10;
    float scopeHeight = h * 0.55f;
    float scopeMid    = scopeTop + scopeHeight * 0.5f;

    // try to grab a fresh buffer from Synth — never block the audio thread
    synth->copyScope(scopeBuffer);

    // centre line
    ofSetColor(60, 60, 60);
    ofSetLineWidth(1);
    ofDrawLine(0, scopeMid, w, scopeMid);

    // waveform
    ofSetColor(80, 220, 100);
    ofSetLineWidth(1.5f);

    ofPolyline line;
    for (int i = 0; i < (int)scopeBuffer.size(); i++) {
        float x = ofMap(i, 0, (int)scopeBuffer.size() - 1, 0, w);
        float y = ofMap(scopeBuffer[i], -1.0f, 1.0f, scopeTop + scopeHeight, scopeTop);
        line.addVertex(x, y);
    }
    line.draw();

    // label
    ofSetColor(100, 100, 100);
    ofDrawBitmapString("SCOPE", 8, scopeTop + 14);
}

// --- FFT spectrum bars (middle portion) ---
void Visualiser::drawSpectrum() {
    float w  = ofGetWidth();
    float h  = ofGetHeight();

    // spectrum lives between 58% and 85% of screen height
    float specTop    = h * 0.58f;
    float specHeight = h * 0.27f;
    float specBottom = specTop + specHeight;

    // ofSoundGetSpectrum returns a pointer to a static array — copy it
    float* raw = ofSoundGetSpectrum(NUM_BINS);

    // log-scale x mapping so low frequencies (where all the music is) get more space
    // bin 0 is DC (skip it), bins 1–255 map onto screen width
    float barW = w / (float)(NUM_BINS - 1);

    for (int i = 1; i < NUM_BINS; i++) {
        // logarithmic x position
        float t    = (float)i / (float)(NUM_BINS - 1);   // 0 to 1
        float xLog = log(1.0f + t * 99.0f) / log(100.0f); // log scale 0 to 1
        float x    = xLog * w;

        float amp  = raw[i];

        // clamp just in case
        if (amp < 0) amp = 0;
        if (amp > 1) amp = 1;

        float barH = amp * specHeight;

        // colour shifts from green (low) to cyan (high)
        float hue = ofMap(i, 1, NUM_BINS - 1, 120, 180);
        ofColor c;
        c.setHsb(hue, 200, 220);
        ofSetColor(c);

        // draw bar from bottom upward
        ofDrawRectangle(x, specBottom - barH, barW, barH);
    }

    // baseline
    ofSetColor(60, 60, 60);
    ofSetLineWidth(1);
    ofDrawLine(0, specBottom, w, specBottom);

    // label
    ofSetColor(100, 100, 100);
    ofDrawBitmapString("SPECTRUM  (log scale)", 8, specTop + 14);

    // frequency tick marks at musically useful spots
    struct Tick { float freq; std::string label; };
    std::vector<Tick> ticks = {
        {100,  "100"},
        {500,  "500"},
        {1000, "1k"},
        {4000, "4k"},
        {8000, "8k"},
    };

    float sampleRate = 44100.0f;
    for (auto& tick : ticks) {
        // bin index for this frequency
        float binF = tick.freq / (sampleRate / (float)(NUM_BINS * 2));
        if (binF < 1 || binF >= NUM_BINS) continue;

        float t    = binF / (float)(NUM_BINS - 1);
        float xLog = log(1.0f + t * 99.0f) / log(100.0f);
        float x    = xLog * w;

        ofSetColor(70, 70, 70);
        ofDrawLine(x, specBottom, x, specBottom + 5);
        ofSetColor(110, 110, 110);
        ofDrawBitmapString(tick.label, x + 2, specBottom + 14);
    }
}

// --- Volume bar (right side) ---
void Visualiser::drawVolumeBar(float volume) {
    float h = ofGetHeight();

    float barX      = ofGetWidth() - 18;
    float barTop    = 10;
    float barBottom = h * 0.85f;
    float barHeight = barBottom - barTop;

    // track background
    ofSetColor(40, 40, 40);
    ofDrawRectangle(barX, barTop, 10, barHeight);

    // filled portion
    float fillH = barHeight * volume;
    ofSetColor(80, 220, 100);
    ofDrawRectangle(barX, barBottom - fillH, 10, fillH);

    // label
    ofSetColor(100, 100, 100);
    ofDrawBitmapString("VOL", barX - 2, barBottom + 14);
}

// --- Active notes display ---
void Visualiser::drawInfo(std::vector<std::string> activeNotes) {
    ofSetColor(200, 200, 200);

    if (activeNotes.empty()) {
        ofDrawBitmapString("Playing: —", 8, 30);
    } else {
        std::string line = "Playing:";
        for (int i = 0; i < (int)activeNotes.size(); i++) {
            line += " " + activeNotes[i];
            if (i < (int)activeNotes.size() - 1) line += " +";
        }
        ofDrawBitmapString(line, 8, 30);
    }
}

// --- Key guide (bottom strip) ---
void Visualiser::drawKeyGuide() {
    float h = ofGetHeight();
    float y = h - 36;

    ofSetColor(50, 50, 50);
    ofDrawRectangle(0, y - 4, ofGetWidth(), 40);

    ofSetColor(150, 150, 150);
    ofDrawBitmapString(
        "White: z=C4  x=D4  c=E4  v=F4  b=G4  n=A4  m=B4  ,=C5   "
        "Black: s=C#  d=D#  g=F#  h=G#  j=A#   "
        "Clips: 1  2  3",
        8, y + 14
    );
}

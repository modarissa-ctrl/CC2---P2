#pragma once
#include "ofMain.h"
#include "Synth.h"
#include <map>

using namespace std;

// InputMapper - translates keyboard and mouse events into Synth calls.
// It uses Synth but doesn't own it (association relationship).
class InputMapper {
public:
    InputMapper(Synth* synth);

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y, int screenW, int screenH);

    // returns a list of note names currently held, so Visualiser can display them
    vector<string> getActiveNoteNames();

private:
    Synth* synth;

    map<int, float>  keyFreqMap;   // which key maps to which frequency
    map<int, int>    keyVoiceMap;  // which key is currently using which voice slot
    map<int, string> keyNoteMap;   // key -> note name (for display)
    int voiceCounter;
    int clipSlots[3];              // voice pool slots reserved for SampleVoice clips

    static const int VOICE_POOL_SIZE = 8;  // match Ronald's pool - update if needed

    void buildKeyMap();
};

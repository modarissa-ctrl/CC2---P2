#include "InputMapper.h"

InputMapper::InputMapper(Synth* s) {
    this->synth = s;
    this->voiceCounter = 0;
    buildKeyMap();
}

void InputMapper::buildKeyMap() {
    // keyboard layout - z x c v b n m are the white keys
    // s d g h j are the black keys (sharps)
    keyFreqMap['z'] = 261.63;  keyNoteMap['z'] = "C4";
    keyFreqMap['s'] = 277.18;  keyNoteMap['s'] = "C#4";
    keyFreqMap['x'] = 293.66;  keyNoteMap['x'] = "D4";
    keyFreqMap['d'] = 311.13;  keyNoteMap['d'] = "D#4";
    keyFreqMap['c'] = 329.63;  keyNoteMap['c'] = "E4";
    keyFreqMap['v'] = 349.23;  keyNoteMap['v'] = "F4";
    keyFreqMap['g'] = 369.99;  keyNoteMap['g'] = "F#4";
    keyFreqMap['b'] = 392.00;  keyNoteMap['b'] = "G4";
    keyFreqMap['h'] = 415.30;  keyNoteMap['h'] = "G#4";
    keyFreqMap['n'] = 440.00;  keyNoteMap['n'] = "A4";
    keyFreqMap['j'] = 466.16;  keyNoteMap['j'] = "A#4";
    keyFreqMap['m'] = 493.88;  keyNoteMap['m'] = "B4";
    keyFreqMap[','] = 523.25;  keyNoteMap[','] = "C5";

    // clip keys use the last 3 slots in the pool
    // TODO: confirm these slot numbers with Ronald
    clipSlots[0] = 5;
    clipSlots[1] = 6;
    clipSlots[2] = 7;
}

void InputMapper::keyPressed(int key) {
    // don't retrigger if the key is already down
    if (keyVoiceMap.count(key)) return;

    if (!keyFreqMap.count(key)) return;

    // cycle through the first 5 voice slots for melodic notes
    int slot = voiceCounter % 5;
    voiceCounter++;

    keyVoiceMap[key] = slot;
    synth->noteOn(slot, keyFreqMap[key]);
}

void InputMapper::keyReleased(int key) {
    if (keyVoiceMap.count(key)) {
        synth->noteOff(keyVoiceMap[key]);
        keyVoiceMap.erase(key);
    }
}

void InputMapper::mouseMoved(int x, int y, int screenW, int screenH) {
    // TODO: add this Wednesday
}

vector<string> InputMapper::getActiveNoteNames() {
    vector<string> names;

    map<int, int>::iterator it;
    for (it = keyVoiceMap.begin(); it != keyVoiceMap.end(); it++) {
        int k = it->first;
        if (keyNoteMap.count(k)) {
            names.push_back(keyNoteMap[k]);
        }
    }

    return names;
}

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
    // Modar - confirmed: Pool = 8, slots 0-4 = melodic, slots 5-7 = SampleVoice
    clipSlots[0] = 5;
    clipSlots[1] = 6;
    clipSlots[2] = 7;
}

void InputMapper::keyPressed(int key) {
    // Modar - adding these to introduce addition different sounds (brief is clear - at least three different sounds playable by user)
    if (key == 'q') { synth->setVoiceType(0); return; }   // sine
    if (key == 'w') { synth->setVoiceType(1); return; }   // pulse
    if (key == 'e') { synth->setVoiceType(2); return; }   // noise


    // keys 1 2 3 trigger the clips
    // Modar - the order of arguments was  i.e. 0, clipSlots[0] instead of clipSlots[0], 0. Tweaked second argument (freq) to float as well
    if (key == '1') { synth->noteOn(clipSlots[0], 0.0f);  return; }
    if (key == '2') { synth->noteOn(clipSlots[1], 0.0f);  return; }
    if (key == '3') { synth->noteOn(clipSlots[2], 0.0f);  return; }

    // don't retrigger if already held
    if (keyVoiceMap.count(key)) return;

    if (!keyFreqMap.count(key)) return;

    // cycle through the first 5 voice slots for melodic notes
    int slot = voiceCounter % 5;
    voiceCounter++;

    keyVoiceMap[key] = slot;
    // Modar - same incorrect argument order - fixed
    synth->noteOn(slot, keyFreqMap[key]);
}

void InputMapper::keyReleased(int key) {
    // clips are one shot so nothing to release
    if (key == '1' || key == '2' || key == '3') return;

    if (keyVoiceMap.count(key)) {
        synth->noteOff(keyVoiceMap[key]);
        keyVoiceMap.erase(key);
    }
}

void InputMapper::mouseMoved(int x, int y, int screenW, int screenH) {
    // mouse x = filter, mouse y = volume
    float cutoff = ofMap(x, 0, screenW, 0.0, 1.0, true);
    synth->setFilterCutoff(cutoff);

    // flipped so top of screen = loud, bottom = quiet
    // not starting from 0 so the sound doesn't disappear completely
    float vol = ofMap(y, screenH, 0, 0.1, 1.0, true);
    synth->setMasterVolume(vol);
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

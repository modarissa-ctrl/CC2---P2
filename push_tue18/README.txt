PUSH: Tuesday 18th — Visualiser waveform scope
===============================================
Target: waveform drawing on screen. With stub Synth it draws a flat line at centre.

New in this push:
  Visualiser.h    <- waveform scope, key guide, note name display
  Visualiser.cpp  <- waveform only, spectrum panel comes push_thu20

What to copy into src/:
  Visualiser.h
  Visualiser.cpp

Wire into ofApp:
  In ofApp.h:              add   Visualiser* vis;
  In ofApp.cpp setup():    add   vis = new Visualiser(synth);
  In ofApp.cpp update():   add   vis->update();
  In ofApp.cpp draw():     add   vis->draw(0, 0, ofGetWidth(), ofGetHeight());
                                 vis->drawInfo(input->getActiveNoteNames(), 10, 10);

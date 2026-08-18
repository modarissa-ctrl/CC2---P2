PUSH: Monday 17th (catch-up push, committed Tue 18) — Shared headers + InputMapper
====================================================================================
Target: keyboard layout complete. Keys z-m + , trigger notes across one octave.

New in this push vs push_sun16:
  Synth.h          <- renamed from proposed_Synth.h (added std:: to vector param, wouldn't compile without it)
  Synth_stub.cpp   <- placeholder so the project builds while the sound engine is being written
  SoundSource.h    <- renamed from proposed_SoundSource.h (no changes)
  Envelope.h       <- placeholder ADSR, full version comes later
  InputMapper.h    <- keyboard to note mapping, association with Synth
  InputMapper.cpp  <- keyboard only, mouse control comes push_wed19

What to copy into src/:
  All 6 files above

Wire into ofApp:
  In ofApp.h:              add   InputMapper* input;
  In ofApp.cpp setup():    add   input = new InputMapper(synth);
  In ofApp.cpp keyPressed(int key):   input->keyPressed(key);
  In ofApp.cpp keyReleased(int key):  input->keyReleased(key);

PUSH: Wednesday 19th — Mouse control + SampleVoice
====================================================
Target: mouse changes tone and volume. Clip keys (1, 2, 3) play pre-recorded sounds.
YD Post: mouse demo video OR clip hunting story (freesound.org search screenshot).

Repo: https://github.com/modarissa-ctrl/CC2---P2

What to copy into src/:
  InputMapper.cpp   <- REPLACES the one from push_mon17 (mouse support added)
  SampleVoice.h     <- NEW
  SampleVoice.cpp   <- NEW

Wire into ofApp (shared file - discuss with Ronald):
  - SampleVoice slots need to be added to Ronald's voice pool in Synth
    Ask Ronald: "Can you add 3 SampleVoice slots to the pool? I'll provide the .h file."
  - In ofApp.cpp keyPressed(): add  input->keyPressed(key);  (already done from mon17)
    clip triggering is handled inside InputMapper when keys 1/2/3 are pressed
    InputMapper will call synth->noteOn() on the SampleVoice slots directly

CLIP FILES - find before Wednesday:
  Source: freesound.org -> filter by "CC0" license (free to use, no attribution needed)
  Download 3 short WAV files (max ~2 seconds each)
  Rename them: clip1.wav, clip2.wav, clip3.wav
  Copy into the project's bin/data/ folder
  These need to be in the zip for submission

Good search terms on freesound.org:
  - "drum hit" CC0
  - "synth stab" CC0
  - "ambient one shot" CC0

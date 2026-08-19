PUSH: Wednesday 19th — Mouse control + SampleVoice
====================================================
Target: mouse changes tone and volume. Clip keys (1, 2, 3) play pre-recorded sounds.

Repo: https://github.com/modarissa-ctrl/CC2---P2

What to copy into src/:
  InputMapper.cpp   <- REPLACES the one from push_mon17 (mouse support added)
  SampleVoice.h     <- NEW
  SampleVoice.cpp   <- NEW

Wire into ofApp (shared file):
  - Ronald: can you add 3 SampleVoice slots (slots 5, 6, 7) to the voice pool in Synth?
    SampleVoice.h is in this folder — it inherits SoundSource so it fits straight in.
  - keyPressed() wiring is already done from Monday's push.
    Keys 1/2/3 go through InputMapper -> synth->noteOn() on those slots directly.

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

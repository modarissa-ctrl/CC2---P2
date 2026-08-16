PUSH: Sunday 16th — Shared API Proposal
========================================

What these files are:
- proposed_SoundSource.h  -> the abstract base class (Ronald implements, Modar inherits SampleVoice from it)
- proposed_Synth.h        -> the 5 methods Modar needs to call from InputMapper and Visualiser
                             Ronald fills in the implementation, Modar just needs the interface agreed

Do NOT start InputMapper or Visualiser until Ronald confirms the Synth.h API.
Once he confirms -> start push_mon17 work immediately.

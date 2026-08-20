#pragma once

// placeholder envelope until Ronald sends the real one
// right now it just passes the sample through at full volume
// no attack, decay, sustain, release - that all comes from Ronald's version
class Envelope {
public:
    Envelope() {
        active = false;
    }

    void noteOn() {
        active = true;
    }

    void noteOff() {
        active = false;
    }

    float nextSample() {
        if (active) {
            return 1.0;
        }
        return 0.0;
    }

private:
    bool active;
};

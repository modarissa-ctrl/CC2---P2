#pragma once

// placeholder envelope until Ronald sends the real one
// right now it just passes the sample through at full volume
// no attack, decay, sustain, release - that all comes from Ronald's version

// Ronald's version - now implemented :-)
class Envelope {
public:
    Envelope();

    void noteOn();
    void noteOff();
    float nextSample();

	bool isActive() const;              // true until the release phase is complete
	void setSampleRate(float sr);
	void setADSR(float attack, float decay, float sustain, float release);   // set the ADSR parameters in seconds and sustain level (0.0 to 1.0)

private:
	enum Stage {
		IDLE,
		ATTACK,
		DECAY,
		SUSTAIN,
		RELEASE
	};

	Stage stage;
	float level;
	float sampleRate;
	float attackTime, decayTime, sustainLevel, releaseTime;
	float attackRate, decayRate, releaseRate;

	void recalcRates();  // recalculate the attack, decay, and release rates based on the current sample rate and ADSR parameters
};

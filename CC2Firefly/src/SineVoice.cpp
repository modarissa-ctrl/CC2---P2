#include "SineVoice.h"
#include <cmath>

float SineVoice::waveform(float phase) {
	// Calculate the sine of the phase, scaled to the range of 0 to 2π since it should be in radians for the sine function. 
	// The phase is expected to be in the range [0, 1], so we multiply by 2π (approximately 6.28318530718) to convert it to radians.
    return sinf(phase * 6.28318530718f);
}

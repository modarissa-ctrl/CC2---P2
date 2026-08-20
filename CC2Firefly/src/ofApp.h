#pragma once

#include "ofMain.h"
#include "Synth.h"
#include "InputMapper.h"
#include "Visualiser.h"

// ofApp owns Synth, InputMapper, and Visualiser. Shared responsibility between Modar and Ronald as per our plan

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		//void mouseDragged(int x, int y, int button);
		//void mousePressed(int x, int y, int button);
		//void mouseReleased(int x, int y, int button);
		//void mouseEntered(int x, int y);
		//void mouseExited(int x, int y);
		//void windowResized(int w, int h);
		//void dragEvent(ofDragInfo dragInfo);
		//void gotMessage(ofMessage msg);

		// Firefly additions to standard template
		// audioOut is called on the sound card's thread, not the main thread.
		void audioOut(ofSoundBuffer& buffer);

private:
	Synth synth;
	InputMapper * inputMapper;
	Visualiser * visualiser;

	ofSoundStream soundStream;
	float phase;			// runs from 0 to 1 per cycle (not 0 to 2*pi)
	float phaseStep;		// how far the phase moves each sample

	int clipsLoaded;
	int sampleRate;
	int bufferSize;
};

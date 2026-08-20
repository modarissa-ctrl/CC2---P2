#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		// Firefly additions to standard template
		void audioOut(ofSoundBuffer& buffer);

private:
	ofSoundStream soundStream;
	float phase;			// runs from 0 to 1 per cycle (not 0 to 2*pi)
	float phaseStep;		// how far the phase moves each sample
		
};

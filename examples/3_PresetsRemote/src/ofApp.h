#pragma once

#include "ofMain.h"

#include "ofxSurfingPresets.h"

/*

	To run this example out-of-the-box,
	due to the addons.make content,
	you must go to the file ofxSurfingPresets.h 
	and to uncomment the line
	#define INCLUDE__OFX_SURFING_CONTROL__OFX_REMOTE_PARAMETERS__SERVER

*/

class ofApp : public ofBaseApp {
public:

	void setup();
	void draw();
	void drawScene();

	ofxSurfingPresets presets;

	//-

	// Scene Parameters 

	ofParameterGroup params;

	ofParameter<float> size1;
	ofParameter<int> size2;
	ofParameter<float> rotation1;
	ofParameter<float> rotation2;
	ofParameter<int> colorIndex;
};

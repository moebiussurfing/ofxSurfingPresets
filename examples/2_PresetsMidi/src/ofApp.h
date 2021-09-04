#pragma once

#include "ofMain.h"

#include "ofxSurfingPresets.h"

class ofApp : public ofBaseApp{

public:
		void setup();
		void draw();
		void drawScene();
		void keyPressed(int key);

		ofxSurfingPresets presets;

		// scene parameters 
		ofParameterGroup params;
		ofParameter<float> size1;
		ofParameter<float> rotation1;
		ofParameter<float> rotation2;
		ofParameter<int> size2;
};

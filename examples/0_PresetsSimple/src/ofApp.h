#pragma once

#include "ofMain.h"

#include "ofxSurfingPresets.h"

class ofApp : public ofBaseApp
{

public:
		void setup();
		void draw();
		void drawScene();

		ofxSurfingPresets presetsManager;

		//--

		// Scene Parameters 

		ofParameterGroup params;

		ofParameter<float> size1;
		ofParameter<int> size2;
		ofParameter<float> rotation1;
		ofParameter<float> rotation2;
		ofParameter<int> indexColor;
};

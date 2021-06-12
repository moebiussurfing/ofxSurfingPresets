#pragma once

#include "ofMain.h"

#include "ofxSurfingPresets.h"

class ofApp : public ofBaseApp{

public:
		void setup();
		void update();
		void draw();
		void exit();
		void keyPressed(int key);

		ofxSurfingPresets presets;

		// scene parameters 
		ofParameter<bool> fill;
		ofParameter<float> lineWidth;
		ofParameter<ofFloatColor> color;
		ofParameter<int> shapeType;
		ofParameter<int> amount;
		ofParameter<int> separation;
		ofParameter<int> size;
		ofParameterGroup params;
};

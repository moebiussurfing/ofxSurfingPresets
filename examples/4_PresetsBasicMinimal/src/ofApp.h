#pragma once

#include "ofMain.h"

/*

	This example shows and example with the minimal expression of a preset selector.
	Kind of a player when you don't need deep edition in first plain.

*/

#include "ofxSurfingPresets.h"
#include "ofxSurfingImGui.h"

class ofApp : public ofBaseApp{

public:
		void setup();
		void update();
		void draw();
		void drawScene();
		void keyPressed(int key);

		ofxSurfingPresets presets;

		//-

		// Scene Parameters 

		ofParameterGroup params;

		ofParameter<float> size1;
		ofParameter<int> size2;
		ofParameter<float> rotation1;
		ofParameter<float> rotation2;
		ofParameter<int> indexColor;

		ofxSurfing_ImGui_Manager guiManager; // -> The ImGui Manager !
		void setupImGui();
		ofParameter<bool> bWindow1{ "Window1", true };
		ofParameter<bool> bWindow2{ "Window2", false};
};

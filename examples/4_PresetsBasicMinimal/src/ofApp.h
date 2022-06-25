#pragma once

#include "ofMain.h"


/*

	This example shows and example with the minimal expression of a preset selector.
	Kind of a player when you don't need deep edition in first plane.

*/


#include "ofxSurfingPresets.h"
#include "ofxSurfingImGui.h"
#include "ofxWindowApp.h"

class ofApp : public ofBaseApp {

public:

	void setup();
	void draw();
	void drawScene();
	void keyPressed(int key);

	ofxSurfingPresets presetsManager;

	//--

	// Scene Parameters 

	ofParameterGroup params;

	ofParameter<float> size1;
	ofParameter<int> size2;
	ofParameter<float> rotation1;
	ofParameter<float> rotation2;
	ofParameter<int> indexColor;

	//--

	ofxSurfing_ImGui_Manager guiManager; // -> The ImGui Manager !
	
	ofParameter<bool> bGui_1{ "Window 1", true };
	ofParameter<bool> bClickerMinimal{ "Clicker Minimal", false };
	ofParameter<bool> bParameters{ "Parameters", false };

	ofParameter<bool> bGui_2{ "Window 2", true };

	//--

	ofxWindowApp windowApp;
};

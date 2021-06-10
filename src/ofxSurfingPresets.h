
#pragma once

#include "ofMain.h"

//#include "ofxGui.h"
#include "ofxSurfingHelpers.h"

#include "ofxSurfingImGui.h"
using namespace ofxSurfing;
//using namespace ofxSurfingHelpers;

class ofxSurfingPresets
{

public:
	bool bDebug = false;

	string nameSelected;

	ofxSurfing_ImGui_Manager guiManager;

	// preset params
	ofParameterGroup params_Preset{ "-1" };
	void addGroup(ofParameterGroup &group) {
		params_Preset = group;
	}

	ofParameterGroup params;
	ofParameterGroup params_Control;
	ofParameterGroup params_Internal;

	ofParameter<bool> bSave;
	ofParameter<bool> bLoad;
	ofParameter<bool> bSetPathPresets;
	ofParameter<bool> bRefresh;
	ofParameter<int> index;

	//----

	ofxSurfingPresets();
	~ofxSurfingPresets();

	void setup();
	void update(ofEventArgs & args);
	void draw(ofEventArgs & args);
	void exit();
	void startup();

	void draw_ImGui();
	bool bOpen0 = true;
	bool bOpen1 = true;
	bool bOpen2 = true;

	//void windowResized(int w, int h);

	void setActive(bool b);
	void setGuiVisible(bool b);
	void setLogLevel(ofLogLevel level);
	void setAutoSave(bool b)
	{
		ENABLE_AutoSave = b;
	}

	//void setKey_MODE_App(int k);

	void load(string path);
	void save(string path);
	
	// files browser
	ofDirectory dir;
	std::string fileName;
	std::string filePath;
	vector<std::string> fileNames;
	void refreshFiles();

	//-

private:

	//int key_MODE_App = OF_KEY_TAB;//default key to switch MODE_App

	// autosave
	ofParameter<bool> ENABLE_AutoSave;
	uint64_t timerLast_Autosave = 0;
	int timeToAutosave = 5000;

	// updating some params before save will trigs also the group callbacks
	//so we disable this callbacks just in case params updatings are required
	//in this case we will need to update gui position param
	bool DISABLE_Callbacks = false;

	//-

	void Changed_Control(ofAbstractParameter &e);

	//-

	// control params
	ofParameter<bool> MODE_Active;
	ofParameter<bool> ENABLE_Debug;
	ofParameter<bool> SHOW_Gui;
	ofParameter<bool> SHOW_GuiPresets;
	//ofParameter<bool> ENABLE_keys;
	//ofParameter<glm::vec2> Gui_Position;
	//ofParameter<bool> SHOW_Help;
	//ofParameter<int> MODE_App;
	//ofParameter<string> MODE_App_Name;

	void Changed_Internal(ofAbstractParameter &e);
	void Changed_Params(ofAbstractParameter &e);

	void setPathGlobal(string s);//must cal before setup. disabled by default
	void setPathPresets(string s);//must cal before setup. disabled by default

	std::string path_Global; // this is to folder all files to avoid mixing with other addons data
	std::string path_Params_Control;
	std::string path_Presets;//this is to folder all files to avoid mixing with other addons data

	//ofxPanel gui_Control;

	//// keys
	 //void keyPressed(ofKeyEventArgs &eventArgs);
	 //void keyReleased(ofKeyEventArgs &eventArgs);
	 //void addKeysListeners();
	 //void removeKeysListeners();

	//// mouse
	 //void mouseDragged(ofMouseEventArgs &eventArgs);
	 //void mousePressed(ofMouseEventArgs &eventArgs);
	 //void mouseReleased(ofMouseEventArgs &eventArgs);
	 //void addMouseListeners();
	 //void removeMouseListeners();
};

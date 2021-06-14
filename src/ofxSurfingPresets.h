
#pragma once

#include "ofMain.h"


/*

+ fix next cycled button
+ fix autosave
+ new preset adds counter at name end
+ add text input to rename preset names
+ batch rename all files

*/


#include "ofxSurfingHelpers.h"
#include "ofxSurfingImGui.h"
//#include "ofxGui.h"

class ofxSurfingPresets
{
	//-

public:
	ofxSurfingPresets();
	~ofxSurfingPresets();

private:
	void setup();
	void update(ofEventArgs & args);
	void draw(ofEventArgs & args);
	void exit();
	void startup();

public:
	void draw_ImGui();
	void draw_ImGui_Minimal();
	void draw_ImGui_Editor();

	//-

private:
	//bool bDebug = false;

	string nameSelected;

	ofxSurfing_ImGui_Manager guiManager;

	// preset params
	ofParameterGroup params_Preset{ "-1" };

	//-

	// api

public:
	void addGroup(ofParameterGroup &group) {
		params_Preset = group;

		//refresh
		startup();
	}

	//-

private:
	// gui params
	ofParameterGroup params; 
	ofParameterGroup params_Control;
	ofParameterGroup params_Internal;

	ofParameter<bool> bCycled;

	ofParameter<bool> bAutoSave;
	int index_PRE = -1;

	ofParameter<bool> bSave;
	ofParameter<bool> bLoad;
	ofParameter<bool> bSetPathPresets;
	ofParameter<bool> bRefresh;
	ofParameter<bool> bDebug;
	ofParameter<bool> bShowParameters;
	ofParameter<int> index;

	bool bOpen0 = true;
	bool bOpen1 = true;
	bool bOpen2 = true;

public:
	void setActive(bool b);
	void setGuiVisible(bool b);
	void setLogLevel(ofLogLevel level);
	void setAutoSave(bool b)
	{
		bAutoSave = b;
	}
	//void setKey_MODE_App(int k);

public:
	void load(int _index);
	void load(string path);
	void save(string path);
	void newPreset();
	void copyPreset();
	void deletePreset();
	void refreshFiles();
	void setPath();

private:
	// files browser
	ofDirectory dir;
	std::string fileName;
	std::string filePath;
	vector<std::string> fileNames;

	//-

private:

	//int key_MODE_App = OF_KEY_TAB;//default key to switch MODE_App

	// autosave
	ofParameter<bool> bAutoSaveTimer;
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

public:
	ofParameter<bool> bGui;
	//ofParameter<bool> bShowParameters;

private:
	ofParameter<bool> MODE_Active;
	ofParameter<bool> ENABLE_Debug;

	//ofParameter<bool> ENABLE_keys;
	//ofParameter<glm::vec2> Gui_Position;
	//ofParameter<bool> SHOW_Help;
	//ofParameter<int> MODE_App;
	//ofParameter<string> MODE_App_Name;

	void Changed_Internal(ofAbstractParameter &e);
	void Changed_Params(ofAbstractParameter &e);

public:
	void setPathGlobal(string s);//must cal before setup. disabled by default
	void setPathPresets(string s);//must cal before setup. disabled by default

private:
	std::string path_Global; // this is to folder all files to avoid mixing with other addons data
	std::string path_Params_Control;
	std::string path_Presets;//this is to folder all files to avoid mixing with other addons data
	
	std::string _ext = ".json";

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


#pragma once

#include "ofMain.h"


/*

+ float clicker full responsible height too
+ add undo engine
+ get copy/drag preset sorting from ofxPresetsManager

+ add text input to rename preset names/pre
+ batch rename all files
+ delete a file should push sorting the gap?

*/


//--------------------------------------

// OPTIONAL

//#define USE_MIDI_PARAMS__SURFING_PRESETS

//--------------------------------------


#ifdef USE_MIDI_PARAMS__SURFING_PRESETS
#include "ofxMidiParams.h"
#endif

#include "ofxSurfingHelpers.h"
#include "ofxSurfingImGui.h"
//#include "ofxGui.h"

#define NUM_KEY_COMMANDS 19

class ofxSurfingPresets
{
	//-

public:
	ofxSurfingPresets();
	~ofxSurfingPresets();

#ifdef USE_MIDI_PARAMS__SURFING_PRESETS
private:
	ofxMidiParams mMidiParams;
#endif
public:
	vector<ofParameter<bool>> notesIndex;
	ofParameterGroup params_PresetToggles{ "Presets" };
	void Changed_Params_PresetToggles(ofAbstractParameter &e);
	void refreshToggleNotes();
	ofParameterGroup & getParametersSelectorToggles() { // to select index preset using bool toggle parameters
		return params_PresetToggles;
	}

private:
	char keyCommands[NUM_KEY_COMMANDS] = {
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
		'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l' };

private:
	bool bAutoDraw; // must be false when multiple ImGui instances created!
public:
	// required to set to false when only one ImGui instance is created. By default is setted to ImGui multi instances
	//--------------------------------------------------------------
	void setImGuiAutodraw(bool b) {
		bAutoDraw = b;
	}

	void setup();

private:
	string nameRoot;

private:
	void update(ofEventArgs & args);
	void draw(ofEventArgs & args);
	void exit();
	void startup();

public:
	void draw_ImGui_Minimal();

private:
	void draw_ImGui();
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
	//--------------------------------------------------------------
	void addGroup(ofParameterGroup &group) { // main group add
		setup();

		params_Preset = group;

		nameRoot = params_Preset.getName();

		//-

		// refresh
		startup();
	}

public:
	void doSaveCurrent();
	void doLoadNext();
	void doLoadPrevious();
	void load(int _index);
	void load(string path);
	void save(string path);
	void doStoreState();
	void doRecallState();
	void doNewPreset();
	void doCopyPreset();
	void doDeletePreset();
	void doClearPresets();
	void doPopulatePresets();
	void doResetParams();
	void doRandomizeParams();
	void doRefreshFiles();
	void setPath();

	//-

private:
	// gui params
	ofParameterGroup params;
	ofParameterGroup params_Control;
	ofParameterGroup params_AppSettings;

	ofParameter<bool> bCycled;

	ofParameter<bool> bAutoSave;
	int index_PRE = -1;

	ofParameter<bool> bSave;
	ofParameter<bool> bLoad;
	ofParameter<bool> bSetPathPresets;
	ofParameter<bool> bRefresh;
	ofParameter<bool> bDebug;
	ofParameter<bool> bShowParameters;
	ofParameter<bool> bShowClicker;
	ofParameter<bool> bShowControl;
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
	ofParameter<bool> bGui; // exposed public to use on external gui's
	//ofParameter<bool> bShowParameters;

private:
	ofParameter<bool> MODE_Active;
	ofParameter<bool> ENABLE_Debug;
	ofParameter<bool> bKeys;
	ofParameter<bool> bFloatingClicker;
	//ofParameter<glm::vec2> Gui_Position;
	//ofParameter<bool> SHOW_Help;
	//ofParameter<int> MODE_App;
	//ofParameter<string> MODE_App_Name;

	ofParameter<int> __amntBtns{ "Max Buttons", 1, 1, 1 };
	ofParameter<bool> __respBtns{ "Responsive", true };
	ofParameter<bool> __bExtra{ "Extra", false };
	ofParameter<bool> __bAutoResize{ "Auto Resize", true };
	ofParameterGroup params_FloatClicker;

	//void Changed_AppSettings(ofAbstractParameter &e);
	//void Changed_Params(ofAbstractParameter &e);

public:
	void setPathGlobal(string s);//must cal before setup. disabled by default
	void setPathPresets(string s);//must cal before setup. disabled by default

private:
	std::string path_Global; // this is to folder all files to avoid mixing with other addons data
	std::string path_Params_Control;
	std::string path_Presets; // this is to folder all files to avoid mixing with other addons data
	std::string path_filePreset;

	std::string _ext = ".json";

	//ofxPanel gui_Control;

//public:
//	void keyPressed(int key);

private:
	// keys
	void keyPressed(ofKeyEventArgs &eventArgs);
	void keyReleased(ofKeyEventArgs &eventArgs);
	void addKeysListeners();
	void removeKeysListeners();

	//// mouse
	 //void mouseDragged(ofMouseEventArgs &eventArgs);
	 //void mousePressed(ofMouseEventArgs &eventArgs);
	 //void mouseReleased(ofMouseEventArgs &eventArgs);
	 //void addMouseListeners();
	 //void removeMouseListeners();
};

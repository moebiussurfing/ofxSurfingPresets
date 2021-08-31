
#pragma once

#include "ofMain.h"


/*

+ fix populate crash
+ float clicker full responsible height too
+ add undo engine. get from ofxPresetsManager
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
private:

#define AMOUNT_KIT_SIZE_DEFAULT 9

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

	//--------------------------------------------------------------
	ofParameterGroup & getParametersSelectorToggles() { // to select index preset using bool toggle parameters triggers!

//#ifdef USE_MIDI_PARAMS__SURFING_PRESETS
//	ofRemoveListener(params_PresetToggles.parameterChangedE(), this, &ofxSurfingPresets::Changed_Params_PresetToggles);
	//notesIndex.clear();
	//params_PresetToggles.clear();
	//for (int i = 0; i <= index.getMax(); i++)
	//{
	//	string n = "Preset ";
	//	//n += ofToString(i < 10 ? "0" : "");
	//	n += ofToString(i);

	//	ofParameter<bool> b{ n, false };
	//	notesIndex.push_back(b);
	//	params_PresetToggles.add(b);
	//}
	//ofAddListener(params_PresetToggles.parameterChangedE(), this, &ofxSurfingPresets::Changed_Params_PresetToggles);
//
//	mMidiParams.clear();
//	mMidiParams.add(params_Preset); // -> to control preset params
//	mMidiParams.add(params_PresetToggles); // -> to select index prest by note/toggle and exclusive
//#endif

		return params_PresetToggles;
	}

	// easy callbacks
	// to retrig when preset not changed but is clicked again.
public:
	bool isRetrigged()
	{
		if (bIsRetrigged)
		{
			bIsRetrigged = false;
			return true;
		}
		else return false;
	}
private:
	bool bIsRetrigged = false;

	//----

	// A. easy callbacks
	// loaded / saved
	// to faster ofApp integration 
	// to check in update() as callback
public:
	//--------------------------------------------------------------
	bool isDoneLoad()
	{
		if (bIsDoneLoad)
		{
			bIsDoneLoad = false;
			return true;
		}
		return false;
	}
private:
	bool bIsDoneLoad = false;

	//--

public:
	//--------------------------------------------------------------
	bool isDoneSave()
	{
		if (bIsDoneSave)
		{
			bIsDoneSave = false;
			return true;
		}
		return false;
	}
private:
	bool bIsDoneSave = false;

	//--

	// B. better callbacks
	// loaded / saved
	// to get (from ofApp) when it happens
public:
	ofParameter<bool> DONE_load;// easy callback to know (in ofApp) that preset LOAD is done 
	ofParameter<bool> DONE_save;// easy callback to know (in ofApp) that preset SAVE is done

	//--

	// C. easy trig-callback
	// used to get alerted when preset has not changed but we like to retrig something
	// in some situation we would like this feature:
	// 1. user clicked a preset box
	// 2. but to the same current loaded preset
	// 3. no need to reload the file settings
	// 4. but we want to use the user box click to trig something
private:
	bool bMustTrig = false;
public:
	//--------------------------------------------------------------
	bool isMustTrig()// trig on select preset or not. this is useful when preset selected not changed, but we want to retrig current preset settings
	{
		if (bMustTrig)
		{
			bMustTrig = false;
			return true;
		}
		else
		{
			return false;
		}
	}

	//----

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
	void draw_ImGui_Floating();
	void draw_ImGui_Parameters();

	void draw_ImGui_MiniClicker();
	//float inner clicker layout
	ofParameter<int> amntBtns{ "Amt Buttons", 4, 1, 4 };
	ofParameter<bool> respBtns{ "Responsive", true };
	//int amntBtns = 4;
	//bool respBtns = true;
	ofParameterGroup params_InnerClicker;


private:
	void draw_ImGui();
	void draw_ImGui_EditorControl();

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
	void setup(ofParameterGroup &group) { // main group add
		addGroup(group);
	}

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
	void saveCurrentPreset(int i = -1) {//legacy api
		doSaveCurrent();
	};

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
	void doClearPresets(bool createOne = true);
	void doPopulatePresets();
	void doPopulatePresetsRandomized();
	void doResetParams();
	void doRandomizeParams();
	void doRandomizeIndex();
	void doRefreshFiles();
	void setPath();

	void setRandomizerBpm(float bpm) {

	}

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
	ofParameter<bool> bGui_Parameters;
	ofParameter<bool> bGui_InnerClicker;
	//ofParameter<bool> bShowControl;
	ofParameter<int> index;

	bool bOpen0 = true;
	bool bOpen1 = true;
	bool bOpen2 = true;

public:

	void setActive(bool b);
	void setGuiVisible(bool b);
	void setLogLevel(ofLogLevel level);

	void setModeAutoSave(bool b) { setAutoSave(b); };//legacy api
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
	//ofParameter<bool> bGui_Parameters;

private:

	ofParameter<bool> bGui_Editor;

private:

	ofParameter<bool> MODE_Active;
	ofParameter<bool> ENABLE_Debug;
	ofParameter<bool> bKeys;
	ofParameter<bool> bGui_FloatingClicker;
	//ofParameter<glm::vec2> Gui_Position;
	//ofParameter<bool> SHOW_Help;
	//ofParameter<int> MODE_App;
	//ofParameter<string> MODE_App_Name;

	//float clicker layout
	ofParameter<int> amntBtnsFloatClicker{ "Max Buttons", 1, 1, 1 };
	ofParameter<bool> respBtnsFloatClicker{ "Responsive", true };
	ofParameter<bool> bExtraFloatClicker{ "Extra", false };
	ofParameter<bool> bAutoResizeFloatClicker{ "Auto Resize", true };
	ofParameterGroup params_FloatClicker;

	//void Changed_AppSettings(ofAbstractParameter &e);
	//void Changed_Params(ofAbstractParameter &e);

public:

	void setPathGlobal(string s); // must cal before setup.
	void setPathPresets(string s); // must call before addGroup/setup. Specially usefull when using multiple preset manager instances or different kits for the same instance.
	//TODO: add a kit selector

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
	//void keyReleased(ofKeyEventArgs &eventArgs);

	void addKeysListeners();
	void removeKeysListeners();

	//// mouse
	 //void mouseDragged(ofMouseEventArgs &eventArgs);
	 //void mousePressed(ofMouseEventArgs &eventArgs);
	 //void mouseReleased(ofMouseEventArgs &eventArgs);
	 //void addMouseListeners();
	 //void removeMouseListeners();
};

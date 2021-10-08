
#pragma once

#include "ofMain.h"


/*

TODO:

+	sorting: ctrl + click = copy, drag ?
		get copy/drag preset sorting from ofxPresetsManager
+	add multi groups
+	add easy tween
+	float clicker full responsible height too
+	add undo engine here.
+	add random engine.

*/


//--------------------------------------

// OPTIONAL

// Midi
//#define INCLUDE__OFX_SURFING_PRESET__OFX_PARAMETER_MIDI_SYNC
//#define INCLUDE__OFX_SURFING_PRESET__OFX_MIDI_PARAMS

#define USE__BASIC_TWEENER

//--------------------------------------


#ifdef INCLUDE__OFX_SURFING_PRESET__OFX_PARAMETER_MIDI_SYNC
#include "ofxSurfingMidi.h"
#endif

#ifdef INCLUDE__OFX_SURFING_PRESET__OFX_MIDI_PARAMS
#include "ofxMidiParams.h"
#endif

#include "ofxSurfingHelpers.h"
#include "ofxSurfingImGui.h"
//#include "ofxGui.h"

#define NUM_KEY_COMMANDS 19

//-

class ofxSurfingPresets
{
private:
#define AMOUNT_KIT_SIZE_DEFAULT 9 // Default common amount presets but can be resized

	//----

public:
	ofxSurfingPresets();
	~ofxSurfingPresets();

	//-

#ifdef INCLUDE__OFX_SURFING_PRESET__OFX_MIDI_PARAMS
private:
	ofxMidiParams mMidiParams;
#endif

#ifdef INCLUDE__OFX_SURFING_PRESET__OFX_PARAMETER_MIDI_SYNC
private:
	ofxSurfingMidi mMidiParams;
#endif

	//-

public:
	vector<ofParameter<bool>> notesIndex;
	ofParameterGroup params_PresetToggles{ "Presets" };
	void Changed_Params_PresetToggles(ofAbstractParameter &e);
	void refreshToggleNotes();

	//--------------------------------------------------------------
	ofParameterGroup & getParametersSelectorToggles() { // To select index preset using bool toggle parameters triggers!

//#ifdef INCLUDE__OFX_SURFING_PRESET__OFX_MIDI_PARAMS
//	ofRemoveListener(params_PresetToggles.parameterChangedE(), this, &ofxSurfingPresets::Changed_Params_PresetToggles);
//notesIndex.clear();
//params_PresetToggles.clear();
//for (int i = 0; i <= index.getMax(); i++)
//{
//	std::string n = "Preset ";
//	//n += ofToString(i < 10 ? "0" : "");
//	n += ofToString(i);
//
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

	// Easy callbacks
	// To retrig when preset not changed but is clicked again.

public:
	//--------------------------------------------------------------
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

	// A. Easy callbacks
	// Loaded / saved
	// To faster ofApp integration 
	// To check in update() as callback

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

	// B. Better Callbacks
	// Loaded / saved
	// To get (from ofApp) when it happens
public:
	ofParameter<bool> DONE_load;// easy callback to know (in ofApp) that preset LOAD is done 
	ofParameter<bool> DONE_save;// easy callback to know (in ofApp) that preset SAVE is done

	//--

	// C. Easy trig-callback
	// Used to get alerted when preset has not changed but we like to retrig something
	// In some situation we would like this feature:
	// 1. User clicked a preset box
	// 2. But to the same current loaded preset
	// 3. No need to reload the file settings
	// 4. But we want to use the user box click to trig something

private:
	bool bMustTrig = false;
public:
	//--------------------------------------------------------------
	bool isMustTrig() // Trig on select preset or not. this is useful when preset selected not changed, but we want to retrig current preset settings
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

	bool bAutoDraw; // Must be false when multiple ImGui instances created!

public:

	// Required to set to false when only one ImGui instance is created. 
	// By default is setted to ImGui multi instances
	//--------------------------------------------------------------
	void setImGuiAutodraw(bool b) {
		bAutoDraw = b;
	}

	void setup();

private:

	std::string nameRoot;

private:

	void update(ofEventArgs & args);
	void draw(ofEventArgs & args);
	void exit();
	void startup();

public:

	void draw_ImGui_Minimal();
	void draw_ImGui_FloatingClicker();
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

	std::string nameSelected;

	ofxSurfing_ImGui_Manager guiManager;

	// Preset Params
	ofParameterGroup params_Preset{ "-1" }; //-> The params that we are storing into each preset file.

	//----

	// Api

public:

	//--------------------------------------------------------------
	void setup(ofParameterGroup &group) { // Main group add
		addGroup(group);
	}

	//--------------------------------------------------------------
	void addGroup(ofParameterGroup &group) { // Main group add
		setup();

		params_Preset = group;

		nameRoot = params_Preset.getName();

		//-
		
		// Basic Tweener
		add(group);

		//-

		// Refresh
		startup();
	}

	//----

private:

	// Extra params to insert to the addon gui but from the parent scope (ofApp)
	ofParameterGroup params_AppExtra{ "-1" };

public:

	//--------------------------------------------------------------
	void addParamsAppExtra(ofParameterGroup &groupExtra) {
		params_AppExtra = groupExtra;
	}

	//----

public:

	void doSaveCurrent();
	//--------------------------------------------------------------
	void saveCurrentPreset(int i = -1) { // Legacy api
		doSaveCurrent();
	};

	//--------------------------------------------------------------
	void doLoad(int _index) {
		load(_index);
	};

	void doLoadNext();
	void doLoadPrevious();
	void load(int _index);
	void load(std::string path);
	void save(std::string path);
	void doStoreState();
	void doRecallState();
	void doNewPreset();
	void doCopyPreset();
	void doDeletePreset(int pos = -1);
	void doClearPresets(bool createOne = true);
	void doPopulatePresets();
	void doPopulatePresetsRandomized();
	void doResetParams();
	void doRandomizeParams();
	void doRandomizeIndex();
	bool doRefreshFiles();
	void doRefreshFilesAndRename();
	void setPath();

	//--------------------------------------------------------------
	void setRandomizerBpm(float bpm) {

	}

	//bool * bReset = NULL;
	//void setResetPtr(bool &b) {
	//	bReset = (bool*)b;
	//}

	// Remote reset
	// Bc each group could have different appropiated reset values

private:
	ofParameter <bool> bReset{ "-1", false };
public:
	//--------------------------------------------------------------
	void setResetPtr(ofParameter<bool> &b) {
		bReset.makeReferenceTo(b);
	}

	//-

public:
	ofParameter<bool> bAutoSave;
	ofParameter<bool> bNewPreset;

private:

	// Gui Params
	ofParameterGroup params;
	ofParameterGroup params_Control;
	ofParameterGroup params_AppSettings;

	ofParameter<bool> bCycled;

	int index_PRE = -1;

	ofParameter<bool> bSave;
	ofParameter<bool> bLoad;
	ofParameter<bool> bSetPathPresets;
	ofParameter<bool> bRefresh;
	ofParameter<bool> bDebug;
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

	//--------------------------------------------------------------
	void setModeAutoSave(bool b) { setAutoSave(b); }; // Legacy Api
	//--------------------------------------------------------------
	void setAutoSave(bool b)
	{
		bAutoSave = b;
	}
	//void setKey_MODE_App(int k);

private:

	// Files Browser
	ofDirectory dir;
	std::string fileName;
	std::string filePath;
	vector<std::string> fileNames;

	//-

private:

	//int key_MODE_App = OF_KEY_TAB;//default key to switch MODE_App

	// Autosave
	ofParameter<bool> bAutoSaveTimer;
	uint64_t timerLast_Autosave = 0;
	int timeToAutosave = 5000;

	// Updating some params before save will trigs also the group callbacks
	// So we disable this callbacks just in case params updatings are required
	// In this case we will need to update gui position param
	bool DISABLE_Callbacks = false;

	//-

	void Changed_Control(ofAbstractParameter &e);

	//-

	// Control Params

public:

	// Exposed public to use on external gui's
	ofParameter<bool> bGui;
	ofParameter<bool> bGui_Editor;
	ofParameter<bool> bGui_FloatingClicker;
	ofParameter<bool> bGui_Parameters;

private:

	ofParameter<bool> MODE_Active;
	ofParameter<bool> ENABLE_Debug;
	ofParameter<bool> bKeys;
	//ofParameter<glm::vec2> Gui_Position;
	//ofParameter<bool> bHelp;
	//ofParameter<int> MODE_App;
	//ofParameter<std::string> MODE_App_Name;

	// Floating Clicker Layout
	ofParameter<int> amntBtnsFloatClicker{ "MaxBut", 1, 1, 1 };
	ofParameter<bool> respBtnsFloatClicker{ "Responsive", true };
	ofParameter<bool> bExtraFloatClicker{ "Extra", false };
	ofParameter<bool> bAutoResizeFloatClicker{ "Auto Resize", true };
	ofParameterGroup params_FloatClicker;

	//void Changed_AppSettings(ofAbstractParameter &e);
	//void Changed_Params(ofAbstractParameter &e);

public:

	void setPathGlobal(std::string s); // Must cal before setup.
	void setPathPresets(std::string s); // Must call before addGroup/setup. Specially usefull when using multiple preset manager instances or different kits for the same instance.
	//TODO: Add a kit selector

private:

	std::string path_Global; // This is to folder all files to avoid mixing with other addons data
	std::string path_Params_Control;
	std::string path_Presets; // This is to folder all files to avoid mixing with other addons data
	std::string path_filePreset;

	std::string _ext = ".json";

	//--------------------------------------------------------------
	std::string getFilepathForIndexPreset(int _index) {
		std::string _si = ofToString(_index);
		if (_index < 10) _si = "0" + _si;
		std::string _ss = nameRoot + "_" + _si;
		std::string _fileName = _ss;
		std::string _filePath = path_Presets + "/" + _ss + _ext;
		ofLogNotice(__FUNCTION__) << _filePath;

		return _filePath;
	}

private:

	// Keys
	void keyPressed(ofKeyEventArgs &eventArgs);
	void addKeysListeners();
	void removeKeysListeners();

	


	//--------------------------------------------------------------

#ifdef USE__BASIC_TWEENER
	
	//TODO:
	// Simple Tweener

//public:
	//// Simple Getters
	//float get(ofParameter<float> &e);
	//int get(ofParameter<int> &e);

private:
	ofParameterGroup mParamsGroup_COPY{ "paramsTweened" };
	
	//----
	
private:

	//--------------------------------------------------------------
	void addParam(ofAbstractParameter& aparam) {

		static string suffix = "_Tween_";

		string _name = aparam.getName();
		ofLogNotice() << __FUNCTION__ << " [ ofAbstractParameter ] \t " << _name;

		//--

		// https://forum.openframeworks.cc/t/ofxparametercollection-manage-multiple-ofparameters/34888/3
		auto type = aparam.type();

		bool isGroup = type == typeid(ofParameterGroup).name();

		bool isFloat = type == typeid(ofParameter<float>).name();
		bool isInt = type == typeid(ofParameter<int>).name();
		bool isBool = type == typeid(ofParameter<bool>).name();

		// vec
		bool isVec2 = type == typeid(ofParameter<glm::vec2>).name();
		bool isVec3 = type == typeid(ofParameter<glm::vec3>).name();
		bool isVec4 = type == typeid(ofParameter<glm::vec4>).name();

		ofLogNotice() << __FUNCTION__ << " " << _name << " \t [ " << type << " ]";

		if (isGroup)
		{
			auto &g = aparam.castGroup();

			for (int i = 0; i < g.size(); i++) {
				addParam(g.get(i));
			}
		}

		// add/queue each param
		// exclude groups to remove from plots
		if (!isGroup) mParamsGroup_COPY.add(aparam);

		//--

		// create a copy group
		// will be the output or target to be use params

		if (isFloat) {
			ofParameter<float> p = aparam.cast<float>();
			ofParameter<float> _p{ _name + suffix, p.get(), p.getMin(), p.getMax() };
			mParamsGroup_COPY.add(_p);
		}
		else if (isInt) {
			ofParameter<int> p = aparam.cast<int>();
			ofParameter<int> _p{ _name + suffix, p.get(), p.getMin(), p.getMax() };
			mParamsGroup_COPY.add(_p);
		}
		else if (isBool) {
			ofParameter<bool> p = aparam.cast<bool>();
			ofParameter<bool> _p{ _name + suffix, p.get() };
			mParamsGroup_COPY.add(_p);
		}

		//TODO:
		//vec
		else if (isVec2) {
			ofParameter<glm::vec2> p = aparam.cast<glm::vec2>();
			ofParameter<glm::vec2> _p{ _name + suffix, p.get(), p.getMin(), p.getMax() };
			mParamsGroup_COPY.add(_p);
		}
		else if (isVec3) {
			ofParameter<glm::vec3> p = aparam.cast<glm::vec3>();
			ofParameter<glm::vec3> _p{ _name + suffix, p.get(), p.getMin(), p.getMax() };
			mParamsGroup_COPY.add(_p);
		}
		else if (isVec4) {
			ofParameter<glm::vec4> p = aparam.cast<glm::vec4>();
			ofParameter<glm::vec4> _p{ _name + suffix, p.get(), p.getMin(), p.getMax() };
			mParamsGroup_COPY.add(_p);
		}

		else {
		}
	}

	//--------------------------------------------------------------
	void add(ofParameterGroup aparams) {
		for (int i = 0; i < aparams.size(); i++) {
			addParam(aparams.get(i));
		}
	}

	//--------------------------------------------------------------
	void add(ofParameter<float>& aparam) {
		addParam(aparam);
	}
	//--------------------------------------------------------------
	void add(ofParameter<bool>& aparam) {
		addParam(aparam);
	}
	//--------------------------------------------------------------
	void add(ofParameter<int>& aparam) {
		addParam(aparam);
	}
	//--------------------------------------------------------------
	void add(ofParameter<glm::vec2>& aparam) {
		addParam(aparam);
	}
	//--------------------------------------------------------------
	void add(ofParameter<glm::vec3>& aparam) {
		addParam(aparam);
	}
	//--------------------------------------------------------------
	void add(ofParameter<glm::vec4>& aparam) {
		addParam(aparam);
	}

	//------------

	// API getters
public:
	// to get the smoothed parameters indiviauly and externaly

	//simplified getters
	//--------------------------------------------------------------
	float get(ofParameter<float> &e) {
		string name = e.getName();
		auto &p = mParamsGroup_COPY.get(name);
		if (p.type() == typeid(ofParameter<float>).name())
		{
			return p.cast<float>().get();
		}
		else
		{
			ofLogError(__FUNCTION__) << "Not expected type: " << name;
			return -1;
		}
	}
	//--------------------------------------------------------------
	int get(ofParameter<int> &e) {
		string name = e.getName();
		auto &p = mParamsGroup_COPY.get(name);
		if (p.type() == typeid(ofParameter<int>).name())
		{
			return p.cast<int>().get();
		}
		else
		{
			ofLogError(__FUNCTION__) << "Not expected type: " << name;
			return -1;
		}
	}

	void updateTweeners();
#endif

};

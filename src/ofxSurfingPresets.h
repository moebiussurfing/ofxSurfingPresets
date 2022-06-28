
#pragma once

#include "ofMain.h"


/*

	TODO:

	+ fix startup grow amount presets!
	+ add / check undo engine workflow.
	+ amount selector for kit populator

	+ simple smoothing fails sometimes...
	+ add multiple group as ofxPresetsManager does.
	+ add memory mode.
		copy from ofxPresetsManager
		not reading from files.
		a vector of json or a json should be fine.
	+ add mode to not set key commands, 
		to avoid letters and be the same as the index.

*/


//--------------------------------------

// OPTIONAL

//--

// 1. MIDI -> Optional. Two Alternatives. 
// (Uncomment One or none)
//#define INCLUDE__OFX_SURFING_PRESET__OFX_MIDI_PARAMS // A -> Recommended
//#define INCLUDE__OFX_SURFING_PRESET__OFX_PARAMETER_MIDI_SYNC // B -> WIP

//--

//TODO:
// 2. Smooth
// (Smooth the params transitions between presets)
// -> Optional. Can be commented to disable simple core smoothing.
//#define USE__OFX_SURFING_PRESETS__BASIC_SMOOTHER 

//--

// 3. Index Player (Is a timed player to auto browse presets index)
// -> Optional. Can be commented to disable player browser add-on.
#define USE__OFX_SURFING_PRESETS__OFX_SURFING_PLAYER 

//--

// 4. Remote
// Server using ofxRemoteParameters
// ->This will serve all the parameters to be controlled by a Flutter based remote app,
// like on your mobile device.Powered by RemoteRemote and ofxRemoteParamaters tools by c-mendoza.
//#define USE__OFX_SURFING_CONTROL__OFX_REMOTE_PARAMETERS__SERVER

//--

// Create Preset Index Selector Toggles
// Useful to easy link to MIDI / OSC triggers by auto populating adreses listeners or MIDI notes.

//--

// 5. Undo Engine
#define USE__OFX_SURFING__OFX_SURFING_UNDO_HELPER // -> Declarations to help integrate into our apps/add-ons

#ifdef USE__OFX_SURFING__OFX_SURFING_UNDO_HELPER 
#include "ofxSurfingUndoHelper.h"
#endif

//--------------------------------------


//--

#include "ofxSurfingHelpers.h"
#include "ofxSurfingImGui.h"
#include "TextBoxWidget.h"

//--

// MIDI

#ifdef INCLUDE__OFX_SURFING_PRESET__OFX_PARAMETER_MIDI_SYNC
#include "ofxSurfingMidi.h"
#define USE__OFX_SURFING_PRESET__MIDI__
#endif

#ifdef INCLUDE__OFX_SURFING_PRESET__OFX_MIDI_PARAMS
#include "ofxMidiParams.h"
#define USE__OFX_SURFING_PRESET__MIDI__
#endif

//--

// Remote Server

#ifdef USE__OFX_SURFING_CONTROL__OFX_REMOTE_PARAMETERS__SERVER
#include "ofxRemoteParameters/Server.h"
#endif

//--

// Player

#ifdef USE__OFX_SURFING_PRESETS__OFX_SURFING_PLAYER 
#include "ofxSurfingPlayer.h"
#endif


//----


class ofxSurfingPresets
{
public:

	ofxSurfingPresets();
	~ofxSurfingPresets();

public:

	void setup();
	void setupGui();

private:

	void update(ofEventArgs& args);
	void exit();
	void startup();

public:

	void draw();//legacy API, could remove
	void drawGui() { draw(); };

	void draw_ImGui_Main();
	void draw_ImGui_Parameters();//will be the main or most used window

	// Minimal
	void draw_ImGui_ClickerMinimal();

	// Clicker Simple (Inner)
	void draw_ImGui_ClickerSimple(bool bHeader/* = true*/, bool bMinimal = false, bool bShowMinimize = true, bool bExtras = false);//inner clicker to fast integrate clicker to an external ImGui panel windows.
	void draw_ImGui_ClickerSimple();//forced simple clicker!

	//--

private:

	std::string name_Root = "-1";

private:

	// Default common amount presetsManager but can be resized
#define AMOUNT_KIT_SIZE_DEFAULT_ 9
	int amountKitOfPresetsSize = AMOUNT_KIT_SIZE_DEFAULT_;

public:

	//--------------------------------------
	void setMaxPresetsAmount(int amt) {//must be called first! before adding groups (who calls setup)
		amountKitOfPresetsSize = amt;
	}

	//----

	// Optional Stuff

	// MIDI

public:

#ifdef INCLUDE__OFX_SURFING_PRESET__OFX_MIDI_PARAMS
	ofxMidiParams surfingMIDI;
#endif

#ifdef INCLUDE__OFX_SURFING_PRESET__OFX_PARAMETER_MIDI_SYNC
	ofxSurfingMidi surfingMIDI;
#endif

#ifdef USE__OFX_SURFING_PRESET__MIDI__
	ofParameterGroup params_MIDI{ "ofxSurfingPresets MIDI" };
	void doRecreateMidi();
#endif

	//--

//#ifdef USE__OFX_SURFING_PRESET__MIDI__
#if defined(USE__OFX_SURFING_PRESET__MIDI__) || defined(USE__OFX_SURFING_CONTROL__OFX_REMOTE_PARAMETERS__SERVER) || defined(USE__OFX_SURFING_PRESETS__INDEX_SELECTOR_TOGGLES)
#define USE_TOGGLE_TRIGGERS
#endif

#ifdef USE_TOGGLE_TRIGGERS

public:

	vector<ofParameter<bool>> notesIndex;
	ofParameterGroup params_PresetToggles{ "Presets" };
	void Changed_Params_PresetToggles(ofAbstractParameter& e);
	void refreshToggleNotes();
	bool bSyncRemote;

	//TODO:
	//--------------------------------------------------------------
	// To select index preset using bool toggle parameters triggers!
	ofParameterGroup& getParametersSelectorToggles() {

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
		//	surfingMIDI.clear();
		//	surfingMIDI.add(params_Preset); // -> to control preset params
		//	surfingMIDI.add(params_PresetToggles); // -> to select index prest by note/toggle and exclusive
		//#endif

		return params_PresetToggles;
	}

#endif

	//----

	// Server

#ifdef USE__OFX_SURFING_CONTROL__OFX_REMOTE_PARAMETERS__SERVER

private:

	ofxRemoteParameters::Server remoteServer;
	ofParameterGroup params_Server;
	void Changed_Params_Preset(ofAbstractParameter& e);

#endif

	//----

	// Player

#ifdef USE__OFX_SURFING_PRESETS__OFX_SURFING_PLAYER 

public:

	SurfingPlayer playerSurfer;

private:

	ofEventListener listener_Play;
	ofEventListener listener_Beat;
	ofParameter<bool> bRandomPlay{ "Random", false };
	std::vector<std::string> randomTypesPlayNames = { "Next Index", "Random Index", "Random Params" };
	ofParameter<int> randomTypePlayIndex{ "Target", 0, 0, 2 };

#endif

	//--

public: // to expose save methods too
//private:

	// Undo Engine
#ifdef USE__OFX_SURFING__OFX_SURFING_UNDO_HELPER 
	ofxSurfingUndoHelper undoManager;
#endif

	//--

	//TODO:
//	// User beat
//private:
//	bool bBeatBang = false;
//public:
//	//--------------------------------------------------------------
//	bool isBeatBang() {
//		if (bBeatBang) {
//			bBeatBang = false;
//			return true;
//		}
//		else {
//			return false;
//		}
//	}
//#endif

	//----

	// Easy callbacks

	//--

	// A. Easy callbacks

	// Loaded / Saved
	// To faster ofApp integration. 
	// To check in update() as simple callback.

public:

	//--------------------------------------------------------------
	bool isDoneLoad()//easy callback to notify when a preset just loaded
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
	bool isDoneSave()//easy callback to notify when a preset just saved
	{
		if (bIsDoneSave)
		{
			bIsDoneSave = false;//catched

			return true;
		}
		return false;
	}

private:

	bool bIsDoneSave = false;

	//----

	// B. Better Callbacks

	// Loaded / saved
	// To get (from ofApp) when it happens

public:

	ofParameter<bool> DONE_load;// easy callback to know (in ofApp) that preset LOAD is done 
	ofParameter<bool> DONE_save;// easy callback to know (in ofApp) that preset SAVE is done

	//--

	// C. Easy ReTrig - callback

	// Used to get alerted when preset has not changed but we like to retrig something
	// In some situation we would like this feature:
	// 1. User clicked a preset box
	// 2. But to the same current loaded preset
	// 3. No need to reload the file settings
	// 4. But we want to use the user box click to trig something

	//--

public:

	// In some scenarios we will prefer to disable this mode.
	// TODO: explain
	//--------------------------------------------------------------
	void setAutoLoadOnReTrig(bool b) {
		bAutoLoadOnReTrig = b;
	}

private:

	bool bAutoLoadOnReTrig = true;

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

	// These methods allows to customize key commands assignments to trig the presets selector.

private:

#define NUM_KEY_COMMANDS 36

	// This is the sorted map of keys:
	// We will select the starting key, then will follow next ones to assign to next preset index command!
	// Then we avoid to collide keycommands when using multiple presets manager instances 
	// or when colliding with other add-ons.

	char keysFullMap[NUM_KEY_COMMANDS] = { // predefined picked keys to assign commands
	'1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
	'z', 'x', 'c', 'v', 'b', 'n', 'm' };

	int keyFirstPos = -1;
	char keyFirstChar = '0';
	vector<char> keyCommandsChars;

	//TODO:
	// add mode without triggers and using just numbers

public:

	//--------------------------------------------------------------
	void setKeyFirstChar(char kChar)//Customizable keys to avoid collide when using multiple presets manager instances!
	{
		int p = getKeyCommandPosition(kChar);
		if (p == -1)
		{
			ofLogError(__FUNCTION__) << "Can't found key: " << kChar;
		}
		else
		{
			ofLogNotice(__FUNCTION__) << "Set key first command (to correlative keys starting) at key: " << kChar;

			setKeyFirstPos(p);

			keyCommandsChars.clear();
			for (size_t i = p; i < NUM_KEY_COMMANDS; i++)
			{
				keyCommandsChars.push_back(keysFullMap[i]);
			}
		}
	}

private:

	//--------------------------------------------------------------
	void setKeyFirstPos(int kPos)
	{
		ofLogNotice(__FUNCTION__) << "Set key first position at index: " << kPos;

		keyFirstPos = kPos;
	}

	//--------------------------------------------------------------
	int getKeyCommandPosition(char key) {
		int pos = -1;
		for (size_t i = 0; i < NUM_KEY_COMMANDS; i++)
		{
			if (key == keysFullMap[i])
			{
				pos = i;
				return pos;
			}
		}
		return pos;
	}

	//--

//private:

	//bool bAutoDraw; // Must be false when multiple ImGui instances created!

	//// TODO: REMOVE. This is deprecated!
	//// Required to set to false when only one ImGui instance is created. 
	//// By default is setted to ImGui multi instances
	////--------------------------------------------------------------
	//void setImGuiAutodraw(bool b) {
	//	bAutoDraw = b;
	//}

private:

	ofParameter<int> amountButtonsPerRowClickerMini{ "Amt Buttons", -1, 1, 4 };
	ofParameter<bool> bResponsiveButtonsClickerSimple{ "Responsive", true };
	ofParameterGroup params_ClickerSimple;

public:

	ofParameter<bool> bMinimize_Editor{ "Minimize", true };

private:

	ofParameter<bool> bMinimize_Params{ "Minimize", true };

private:

	void draw_ImGui();
	void draw_ImGui_Editor();

public:

	void draw_ImGui_ToolsPreset(bool bMini = true);
	void draw_ImGui_ToolsKit();

	//-

private:

	std::string nameSelected;

	ofxSurfing_ImGui_Manager guiManager;

	// Preset Params
	ofParameterGroup params_Preset{ "-1" }; //-> The params that we are storing into each preset file.

public:

	ofParameter<bool> bMinimize{ "Minimize", true }; // to to mirror the gui manager internal bool and made it public.

	//----

	// API

public:

	//--------------------------------------------------------------
	void setup(ofParameterGroup& group) { // Main group add
		addGroup(group);
	}

	//--------------------------------------------------------------
	void addGroup(ofParameterGroup& group) { // Main group add. WARNING! You must add only one group. Call this only once! Setup wil be called here!
		setup();

		params_Preset = group;

		if (name_Root == "-1") name_Root = params_Preset.getName();

		//-

		// Simple Smoother
#ifdef USE__OFX_SURFING_PRESETS__BASIC_SMOOTHER
		addSmooth(group);

		addParamsAppExtra(params_SmoothControl);
#endif

#ifdef USE__OFX_SURFING_PRESETS__OFX_SURFING_PLAYER
		playerSurfer.setName(name_Root);
#endif
		//--

		// Undo Manager
#ifdef USE__OFX_SURFING__OFX_SURFING_UNDO_HELPER 

		undoManager.setPathGlobal(name_Root);
		undoManager.setup(params_Preset);
#endif

		//-

		//TODO:
		// Should modify to allow queue more groups.
		// Now we can add one single group!

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
	void addParamsAppExtra(ofParameterGroup& groupExtra) {
		params_AppExtra = groupExtra;
	}

	//----

public:

	void save(std::string path);
	void load(int _index);
	void load(std::string path);

	void doSaveCurrent();
	//--------------------------------------------------------------
	void saveCurrentPreset(int i = -1) { // Legacy api. TODO: not using index 
		doSaveCurrent();
	};

	//--------------------------------------------------------------
	void doLoad(int _index) {
		load(_index);
	};

	//--------------------------------------------------------------
	void doReload() {
		//load(index);
		filePath = getFilepathForIndexPreset(index);
		load(filePath);
	};

	void doLoadNext();
	void doLoadPrevious();
	void doStoreState();//is a cache state to compare with a new editing version
	void doRecallState();//available to restore cache states to undo editing version
	void doNewPreset();
	//void doCopyPreset();
	void doDeletePreset(int pos = -1);
	void doClearPresets(bool createOne = true);

	//--

	// Tools Helpers

	// set the index without calling callbacks. 
	// to "scripting purposes only". 
	// i.e. 
	// populate presets with customized parameters values!
	//--------------------------------------------------------------
	void doSavePresetIndexOffline(int i)
	{
		index.setWithoutEventNotifications(i);
		filePath = getFilepathForIndexPreset(index);
		save(filePath);
	}

	//--

	// Kit helpers
	void doPopulatePresets(int amount = -1);
	void doPopulatePresetsRandomized();

	// Preset Helpers
	void doResetParams(bool bSilent = false);
	void doRandomizeParams(bool bSilent = false);//true for silent mode for "scripting" purposes

	// Helpers
	void doRandomizeIndex();

	//void doSortParams(int i);//TODO:? useful when params are kind of index selectors?

	//--

	bool doRefreshFiles();
	void doRefreshFilesAndRename();

private:

	void setPath(); // Open dialog to select a path manually.

	bool bResetDefined = false;

public:

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

	ofParameter <bool> bReset{ "-1", false }; // TODO: Must set a name when defined a callback function..
	//TODO: we could use a memory state save/load(reset) that we will use as "Reset State"

public:

	//--------------------------------------------------------------
	void setResetPtr(ofParameter<bool>& b) {
		bReset.makeReferenceTo(b);
	}

	//--

public:

	// Exposed to include on GUI's

	ofParameter<bool> bEditMode; // aka bAutoSave
	ofParameter<bool> bNewPreset;
	ofParameter<bool> bSave;

	ofParameter<bool> bGui_ClickerSimple;

public:

	ofParameter <bool> bAutoResize{ "-1", false };

	//--

private:

	// Gui Params
	ofParameterGroup params;
	ofParameterGroup params_Control;
	ofParameterGroup params_AppSettings;

	ofParameter<bool> bCycled;

	int index_PRE = -1;

	ofParameter<bool> bLoad;
	ofParameter<bool> bSetPathPresets;
	ofParameter<bool> bRefresh;
	ofParameter<bool> bDebug;

	bool bOpen0 = true;
	bool bGui_OverlayControl = true;
	bool bGui_OverlayMixer = true;

public:

	ofParameter<int> index; // Current selected preset index

	//--

public:

	//void setName(string name);
	void setActive(bool b);
	void setGuiVisible(bool b);
	//--------------------------------------------------------------
	void setGuiVisibleToggle() { bGui = !bGui; }
	void setLogLevel(ofLogLevel level);

	//--------------------------------------------------------------
	void setEnableKeySpace(bool b) {
		bKeySpace = b;
	}

	//--------------------------------------------------------------
	void setEnableKeysArrows(bool b) {
		bKeysArrows = b;
	}

	//--------------------------------------------------------------
	void setEnableKeys(bool b) {
		bKeys = b;
		setActive(bKeys);
	}

	//--------------------------------------------------------------
	void setModeAutoSave(bool b) { setAutoSave(b); }; // Legacy Api
	//--------------------------------------------------------------
	void setAutoSave(bool b)
	{
		bEditMode = b;
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

	//// Autosave
	//ofParameter<bool> bAutoSaveTimed;
	//uint64_t timerLast_Autosave = 0;
	//int timeToAutosave = 5000;

	// Updating some params before save will trigs also the group callbacks
	// So we disable this callbacks just in case params updatings are required
	// In this case we will need to update gui position param
	bool bDISABLE_CALLBACKS = false;

	//-

	void Changed_Control(ofAbstractParameter& e);

	//-

	// Control Params

public:

	// Exposed public to use on external Gui's
	ofParameter<bool> bGui;//main
	ofParameter<bool> bGui_Global;
	ofParameter<bool> bGui_Editor;
	ofParameter<bool> bGui_Parameters;

public:

	ofParameter<bool> bKeys; // public to be shared to link with external keys toggles.
	// that's useful when using many add-ons working together.

private:

	ofParameter<bool> bKeySpace;
	ofParameter<bool> bKeysArrows;

	// Floating Clicker Layout
	ofParameter<int> amountButtonsPerRowClicker{ "ButtsRow", -1, 1, 1 };
	ofParameter<bool> bResponsiveButtonsClicker{ "Responsive", true };
	ofParameter<bool> bExtra_Main{ "Extra", false };
	ofParameter<bool> bAutoResize_Clicker{ "Auto Resize ", true };
	ofParameterGroup params_Clicker;

public:

	//--------------------------------------------------------------
	void setAmountButtonsPerRowClickers(int num) // amount of preset buttons per row on matrix clickers
	{
		num = MIN(num, index.getMax() - index.getMin());
		amountButtonsPerRowClicker = num;
		amountButtonsPerRowClickerMini = num;
	}

	//void Changed_AppSettings(ofAbstractParameter &e);
	//void Changed_Params(ofAbstractParameter &e);

public:

	// customize amount of presets per row on the preset clicker
	//--------------------------------------------------------------
	void setMaxPresetsAmountPerRowClickerFloat(int amount)
	{
		amountButtonsPerRowClicker = amount;
	}
	//--------------------------------------------------------------
	void setMaxPresetsAmountPerRowClickerMini(int amount)
	{
		amountButtonsPerRowClickerMini = amount;
	}

public:

	//--------------------------------------------------------------
	void setPlayerPlay(bool b)
	{
#ifdef USE__OFX_SURFING_PRESETS__OFX_SURFING_PLAYER 
		playerSurfer.bPlay = b;
		return;
#endif
		ofLogWarning(__FUNCTION__) << "Player module is disabled!";
	}

	//--------------------------------------------------------------
	void setName(std::string s)// Customize global name to avoid collide with other preset manager instances
	{
		//if (s == "-1") s = "PRESETS EDITOR";
		//else
		//{
		//	n = name_Root;
		//	n += " EDITOR";
		//}

		name_Root = s;

		bGui.setName(s);

		//#ifdef USE__OFX_SURFING_PRESETS__OFX_SURFING_PLAYER 
		//		playerSurfer.setName(s);
		//#endif
	}

	//--

	void setPathGlobal(std::string s); // Must cal before setup.
	void setPathPresets(std::string s); // Must call before addGroup/setup. Specially useful when using multiple preset manager instances or different kits for the same instance.
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
		std::string _ss = name_Root + "_" + _si;
		std::string _fileName = _ss;
		std::string _filePath = path_Presets + "/" + _ss + _ext;
		ofLogNotice(__FUNCTION__) << _filePath;

		return _filePath;
	}

private:

	// Keys
	void keyPressed(ofKeyEventArgs& eventArgs);
	void keyReleased(ofKeyEventArgs& eventArgs);
	void addKeysListeners();
	void removeKeysListeners();

	bool bKeyCtrl = false;
	bool bKeyAlt = false;

	//--------------------------------------------------------------

	//TODO:

	// Simple Smoother

#ifdef USE__OFX_SURFING_PRESETS__BASIC_SMOOTHER
	ofParameter<bool> bSmooth{ "Smooth", false };
	ofParameter<float> smoothSpeed{ "Speed", 0.f, 0.f, 1.f };
	ofParameterGroup params_SmoothControl{ "_Smoother", bSmooth, smoothSpeed };

	float MAX_CLAMP_SMOOTH = 0.85f;
	float sp;

private:

	ofParameterGroup params_Preset_Smoothed{ "params_Tweened" };

	std::string suffix = "";//to append to the soomthed copied params
	//std::string suffix = "_Tween_";

	//----

private:

	//--------------------------------------------------------------
	void addSmoothParam(ofAbstractParameter& aparam) {

		// https://forum.openframeworks.cc/t/ofxparametercollection-manage-multiple-ofparameters/34888/3

		std::string _name = aparam.getName();
		auto type = aparam.type();

		bool isGroup = type == typeid(ofParameterGroup).name();
		bool isFloat = type == typeid(ofParameter<float>).name();
		bool isInt = type == typeid(ofParameter<int>).name();
		//bool isBool = type == typeid(ofParameter<bool>).name();
		bool isVec2 = type == typeid(ofParameter<glm::vec2>).name();// vec
		bool isVec3 = type == typeid(ofParameter<glm::vec3>).name();
		bool isVec4 = type == typeid(ofParameter<glm::vec4>).name();

		ofLogNotice() << __FUNCTION__ << " [ ofAbstractParameter ] \t " << _name << " \t [ " << type << " ]";

		//--

		// Group
		// Do it recursively..
		if (isGroup)
		{
			auto& g = aparam.castGroup();

			for (int i = 0; i < g.size(); i++) { // ofAbstractParameters
				addSmoothParam(g.get(i));
			}
		}

		// Add/queue each param
		// But exclude groups structure
		if (!isGroup) params_Preset_Smoothed.add(aparam);

		//--

		// Create a copied group of params to apply a copied and smoothed "version" of the params.
		// Will be the output or target to be use params.
		// But we will must to read them manually using a method:
		/*
			// Example:
			// Draw()
			static float _size1;
			static int _size2;
			_size1 = presetsManager.get(size1);
			_size2 = presetsManager.get(size2);
		*/

		if (isFloat) {
			ofParameter<float> p = aparam.cast<float>();
			ofParameter<float> _p{ _name + suffix, p.get(), p.getMin(), p.getMax() };
			params_Preset_Smoothed.add(_p);
		}

		else if (isInt) {
			ofParameter<int> p = aparam.cast<int>();
			ofParameter<int> _p{ _name + suffix, p.get(), p.getMin(), p.getMax() };
			params_Preset_Smoothed.add(_p);
		}

		//else if (isBool) {
		//	ofParameter<bool> p = aparam.cast<bool>();
		//	ofParameter<bool> _p{ _name + suffix, p.get() };
		//	params_Preset_Smoothed.add(_p);
		//}

		//TODO:

		// vec
		else if (isVec2) {
			ofParameter<glm::vec2> p = aparam.cast<glm::vec2>();
			ofParameter<glm::vec2> _p{ _name + suffix, p.get(), p.getMin(), p.getMax() };
			params_Preset_Smoothed.add(_p);
		}
		else if (isVec3) {
			ofParameter<glm::vec3> p = aparam.cast<glm::vec3>();
			ofParameter<glm::vec3> _p{ _name + suffix, p.get(), p.getMin(), p.getMax() };
			params_Preset_Smoothed.add(_p);
		}
		else if (isVec4) {
			ofParameter<glm::vec4> p = aparam.cast<glm::vec4>();
			ofParameter<glm::vec4> _p{ _name + suffix, p.get(), p.getMin(), p.getMax() };
			params_Preset_Smoothed.add(_p);
		}

		// Unknown
		else {
			ofLogError(__FUNCTION__) << "Not the expected type for ofParam: " << _name;
		}
	}

	//--------------------------------------------------------------
	void addSmooth(ofParameterGroup aparams) {
		for (int i = 0; i < aparams.size(); i++) {
			addSmoothParam(aparams.get(i));
		}
	}
	//--------------------------------------------------------------
	void addSmooth(ofParameter<float>& aparam) {
		addSmoothParam(aparam);
	}
	//--------------------------------------------------------------
	void addSmooth(ofParameter<bool>& aparam) {
		addSmoothParam(aparam);
	}
	//--------------------------------------------------------------
	void addSmooth(ofParameter<int>& aparam) {
		addSmoothParam(aparam);
	}
	//--------------------------------------------------------------
	void addSmooth(ofParameter<glm::vec2>& aparam) {
		addSmoothParam(aparam);
	}
	//--------------------------------------------------------------
	void addSmooth(ofParameter<glm::vec3>& aparam) {
		addSmoothParam(aparam);
	}
	//--------------------------------------------------------------
	void addSmooth(ofParameter<glm::vec4>& aparam) {
		addSmoothParam(aparam);
	}

	//------------

	// API getters

public:

	// Simplified getters
	// To get the smoothed single parameters.
	// Notice that when smooth is disabled, we will get the raw values without smoothing.
	// (Then, should be the same than use the original ofParams.)

	//TODO: remake templated
	//template<typename ParameterType>
	//inline bool AddVSlider(ofParameter<ParameterType>& parameter, ImVec2 sz = ImVec2(-1.f, -1.f), bool bNoName = false, bool bNoNumber = false)

	//--------------------------------------------------------------
	float get(ofParameter<float>& e) { // Gets smoothed value for passed param. Will use his name and search into param group.
		std::string name = e.getName();

		if (bSmooth) {
			auto& p = params_Preset_Smoothed.get(name); // Smoothed
			if (p.type() == typeid(ofParameter<float>).name())
			{
				return p.cast<float>().get();
			}
			else
			{
				ofLogError(__FUNCTION__) << "Not the expected type: " << name;
				return -1;
			}
		}
		else {
			auto& p = params_Preset.get(name); // Raw
			if (p.type() == typeid(ofParameter<float>).name())
			{
				return p.cast<float>().get();
			}
			else
			{
				ofLogError(__FUNCTION__) << "Not the expected type: " << name;
				return -1;
			}
		}
	}

	//--------------------------------------------------------------
	int get(ofParameter<int>& e) { // Gets smoothed value for passed param. Will use his name and search into param group.
		std::string name = e.getName();

		if (bSmooth) {
			auto& p = params_Preset_Smoothed.get(name); // Smoothed
			if (p.type() == typeid(ofParameter<int>).name())
			{
				return p.cast<int>().get();
			}
			else
			{
				ofLogError(__FUNCTION__) << "Not the expected type: " << name;
				return -1;
			}
		}
		else {
			auto& p = params_Preset.get(name); // Raw
			if (p.type() == typeid(ofParameter<int>).name())
			{
				return p.cast<int>().get();
			}
			else
			{
				ofLogError(__FUNCTION__) << "Not the expected type: " << name;
				return -1;
			}
		}
	}

	void updateSmoother();
	void updateSmoothParam(ofAbstractParameter& aparam);
	// To allow recursive group levels using a funtion to each param

#endif

#ifndef USE__OFX_SURFING_PRESETS__BASIC_SMOOTHER

public:

	//--------------------------------------------------------------
	float get(ofParameter<float>& e) { // Gets raw value for passed param. Will use his name and search into param group.
		std::string name = e.getName();

		{
			auto& p = params_Preset.get(name); // Raw
			if (p.type() == typeid(ofParameter<float>).name())
			{
				return p.cast<float>().get();
			}
			else
			{
				ofLogError(__FUNCTION__) << "Not the expected type: " << name;
				return -1;
			}
		}
	}

	//--------------------------------------------------------------
	int get(ofParameter<int>& e) { // Gets raw value for passed param. Will use his name and search into param group.
		std::string name = e.getName();
		{
			auto& p = params_Preset.get(name); // Raw
			if (p.type() == typeid(ofParameter<int>).name())
			{
				return p.cast<int>().get();
			}
			else
			{
				ofLogError(__FUNCTION__) << "Not the expected type: " << name;
				return -1;
			}
		}
	}

#endif

private:

	bool bNoSettingsFileFound = false;

	//----

private:

	TextBoxWidget textBoxWidget;
	void buildHelp();
};

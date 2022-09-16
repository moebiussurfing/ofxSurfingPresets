#include "ofxSurfingPresets.h"


//--------------------------------------------------------------
ofxSurfingPresets::ofxSurfingPresets()
{
	ofAddListener(ofEvents().update, this, &ofxSurfingPresets::update);
	//ofAddListener(ofEvents().draw, this, &ofxSurfingPresets::draw, OF_EVENT_ORDER_AFTER_APP);

	path_Global = "ofxSurfingPresets/"; // this is to folder all files to avoid mixing with other addons data
	path_Presets = "ofxSurfingPresets/Presets";
	path_Params_Control = "ofxSurfingPresets_Settings.xml";
	path_filePreset = "Preset";

	bDISABLE_CALLBACKS = true;

	setActive(true); // add key and mouse listeners

	//-

	// Customizable Key commands
	// '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
	// 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
	// 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
	// 'z', 'x', 'c', 'v', 'b', 'n', 'm'

	setKeyFirstChar('0');
	//setKeyFirstChar('1');
}

//--------------------------------------------------------------
ofxSurfingPresets::~ofxSurfingPresets()
{
	setActive(false); // remove keys and mouse listeners

	ofRemoveListener(ofEvents().update, this, &ofxSurfingPresets::update);

	// Remove params callbacks listeners
	ofRemoveListener(params_Control.parameterChangedE(), this, &ofxSurfingPresets::Changed_Control);

	//#ifdef USE__OFX_SURFING_PRESET__MIDI__
#if defined(USE__OFX_SURFING_PRESET__MIDI__) || defined(USE__OFX_SURFING_CONTROL__OFX_REMOTE_PARAMETERS__SERVER)
	ofRemoveListener(params_PresetToggles.parameterChangedE(), this, &ofxSurfingPresets::Changed_Params_PresetToggles);
#endif

	ofxSurfingHelpers::saveGroup(params_AppSettings, path_Global + path_Params_Control);

	////TODO:
	//ofxSurfingHelpers::saveGroup(params_Preset, path_Global + path_filePreset + _ext);

	// Remote
	// Server
#ifdef USE__OFX_SURFING_CONTROL__OFX_REMOTE_PARAMETERS__SERVER
	ofRemoveListener(params_Preset.parameterChangedE(), this, &ofxSurfingPresets::Changed_Params_Preset);
#endif

	exit();
}

//TODO:
#ifdef USE_TOGGLE_TRIGGERS
//--------------------------------------------------------------
void ofxSurfingPresets::refreshToggleNotes()
{
	if (bDISABLE_CALLBACKS) return;

	// Sets to true the respective toggle for current index and set to false for the others.

	for (int i = 0; i <= index.getMax() && i < notesIndex.size(); i++)
	{
		notesIndex[i].set(false);
	}
	if (index <= index.getMax() && index < notesIndex.size())
		notesIndex[index].set(true);

#ifdef USE__OFX_SURFING_CONTROL__OFX_REMOTE_PARAMETERS__SERVER
	bSyncRemote = true;
#endif
}
#endif

//--------------------------------------------------------------
void ofxSurfingPresets::setup()
{
	// It's auto called after addGroup!

	// Log Mode
	//ofSetLogLevel("ofxSurfingPresets", OF_LOG_NOTICE);
	//ofSetLogLevel("ofxSurfingPresets", OF_LOG_SILENT);

	//--

	// Params 

	bGui_Global.set("_Gui Global_", true);
	bGui_Global.setSerializable(false);

	bGui.set("PRESETS", true);
	bGui_Editor.set("EDITOR", true);
	bGui_ClickerSimple.set("Clicker Simple", false);
	bGui_Parameters.set("PARAMETERS", false);

	index.set("Index", 0, 0, 0);

	bCycled.set("Cycled", true);
	bSave.set("SAVE", false);
	bLoad.set("LOAD", false);

	bEditMode.set("EDIT", true);
	//bEditMode.set("AUTOSAVE", true);
	//bAutoSaveTimed.set("AutoSaver", false);//not used

	bNewPreset.set("NEW", false);
	bSetPathPresets.set("PATH", false);
	bRefresh.set("REFRESH", false);

	bDebug.set("Debug", true);
	bKeys.set("Keys", true);
	bKeySpace.set("Key Space", true);
	bKeysArrows.set("Keys Arrows", true);

	//-

	// To callback 

	params_Control.clear();
	params_Control.setName("PRESETS CONTROL");
	params_Control.add(index);
	params_Control.add(bSave);
	params_Control.add(bNewPreset);
	params_Control.add(bLoad);
	params_Control.add(bSetPathPresets);
	params_Control.add(bRefresh);
	params_Control.add(bMinimize);

	params_Control.add(bGui);
	params_Control.add(bGui_Editor);
	params_Control.add(bGui_Parameters);

#ifdef USE__OFX_SURFING_PRESETS__OFX_SURFING_PLAYER
	if (!bDisablePlayer) {
		params_Control.add(playerSurfer.bGui);
	}
#endif

	ofAddListener(params_Control.parameterChangedE(), this, &ofxSurfingPresets::Changed_Control);

	//--

	// Params AppSettings

	params_AppSettings.clear();
	params_AppSettings.setName("ofxSurfingPresets_AppSettings");

	params_AppSettings.add(bGui_Editor);
	params_AppSettings.add(bGui_Parameters);
	params_AppSettings.add(bGui_ClickerSimple);
	params_AppSettings.add(index);
	params_AppSettings.add(bEditMode);
	params_AppSettings.add(bCycled);
	params_AppSettings.add(bKeys);
	params_AppSettings.add(bMinimize);

	//params_AppSettings.add(ui.bMinimize);
	//params_AppSettings.add(bMinimize_Params);
	//params_AppSettings.add(bMinimize_Editor);
	//params_AppSettings.add(ui.bExtra);
	//params_AppSettings.add(ui.bAutoResize);

#ifdef USE__OFX_SURFING_PRESETS__OFX_SURFING_PLAYER					
	if (!bDisablePlayer) {
		params_AppSettings.add(randomTypePlayIndex);
		//params_AppSettings.add(playerSurfer.bGui_WidgetBeat);
	}
#endif

	//--

	// MIDI
#ifdef USE__OFX_SURFING_PRESET__MIDI__
	params_AppSettings.add(surfingMIDI.bGui);
#endif

	// Simple Smoother
#ifdef USE__OFX_SURFING_PRESETS__BASIC_SMOOTHER
	params_AppSettings.add(params_SmoothControl);
#endif

	//----

	// Link
	bAutoResize.makeReferenceTo(ui.bAutoResize);
	bKeys.makeReferenceTo(ui.bKeys);

	//--

	// Player

#ifdef USE__OFX_SURFING_PRESETS__OFX_SURFING_PLAYER
	if (!bDisablePlayer)
	{
		playerSurfer.bGui = false; // hidden by default 
		playerSurfer.setPathGlobal(path_Global);
	}
#endif

	//----

	params_Clicker.clear();
	params_Clicker.setName("CLICKER");
	params_Clicker.add(amountButtonsPerRowClicker);
	params_Clicker.add(bResponsiveButtonsClicker);
	params_Clicker.add(bAutoResize_Clicker);
	params_Clicker.add(bMinimize_Editor);
	//params_Clicker.add(bExtra_Main);

	params_AppSettings.add(params_Clicker);

	//-

	params_ClickerSimple.clear();
	params_ClickerSimple.setName("CLICKER SIMPLE");
	params_ClickerSimple.add(amountButtonsPerRowClickerMini);
	params_ClickerSimple.add(bResponsiveButtonsClickerSimple);

	params_AppSettings.add(params_ClickerSimple);

	//--

	// External extra or smooth params
	if (params_AppExtra.getName() != "-1") params_AppSettings.add(params_AppExtra);

	// Exclude
	bSave.setSerializable(false);
	bNewPreset.setSerializable(false);
	bLoad.setSerializable(false);
	bSetPathPresets.setSerializable(false);
	bRefresh.setSerializable(false);

	//--

	// All
	params.clear();
	params.setName("ALL PARAMS");
	params.add(params_Control);
	params.add(params_AppSettings);

	//--

	// Gui
	setupGui();

	//--

	// Files
	doRefreshFiles();

	//--

	// Help
	textBoxWidget.setPath(path_Global + "HelpBox_" + name_Root + "/");//customize path before call setup
	textBoxWidget.setup();
	textBoxWidget.bGui.makeReferenceTo(ui.bHelpInternal);

	//--

	//startup(); // Auto called from AddGroup
}

//--------------------------------------------------------------
void ofxSurfingPresets::buildHelp()
{
	std::string helpInfo = "";

	helpInfo += "PRESETS \n";
	helpInfo += "HELP \n\n";

	helpInfo += "KEY COMMANDS \n\n";

	if (!bKeys) {
		helpInfo += "Keys toggle is disabled. \n";
		helpInfo += "Enable that toggle! \n";
	}
	else {
		helpInfo += "G                GUI \n";
		helpInfo += "I                HELP INFO \n";
		helpInfo += "\n";

		helpInfo += "LOAD \n";
		helpInfo += "PRESET \n\n";

		helpInfo += "MOUSE CLICK \n";
		helpInfo += "+CTRL            COPY \n";
		helpInfo += "+ALT             SWAP \n";
		helpInfo += "\n";
		helpInfo += "1-9              BROWSE \n";
		helpInfo += "< > \n";
		helpInfo += "SPACE            NEXT \n";
		helpInfo += "+CTRL            PLAY \n";
		helpInfo += "\n";
		helpInfo += "BACKSPACE        RESET \n";
		helpInfo += "RETURN           RANDOM \n";
		helpInfo += "\n";
		helpInfo += "COMPARE MEM \n";
		helpInfo += "S                STORE \n";
		helpInfo += "R                RECALL \n";
	}

	textBoxWidget.setText(helpInfo);
}

//--------------------------------------------------------------
void ofxSurfingPresets::setupGui()
{
	ui.setName("Presets");
	// customizing name helps to organize folder settings files on bin/data/  
	// when using multiple GUI instances!

	//--

	ui.setWindowsMode(IM_GUI_MODE_WINDOWS_SPECIAL_ORGANIZER);
	ui.setup();

	ui.addWindowSpecial(bGui);
	ui.addWindowSpecial(bGui_Parameters);
	ui.addWindowSpecial(bGui_Editor);

	//#ifdef USE__OFX_SURFING_PRESETS__OFX_SURFING_PLAYER
	//	//if(!bDisablePlayer)
	//	//ui.addWindowSpecial(playerSurfer.bGui);
	//#endif

	ui.startup();

	//--

	//TODO:
	// Linked to be exposed public. 
	// could be useful in some scenarios.
	ui.bMinimize.makeReferenceTo(bMinimize);
	//bMinimize.makeReferenceTo(ui.bMinimize);
}

#ifdef USE__OFX_SURFING_PRESET__MIDI__

//--------------------------------------------------------------
void ofxSurfingPresets::doRecreateMidi()
{
	ofLogNotice(__FUNCTION__);

#ifdef INCLUDE__OFX_SURFING_PRESET__OFX_MIDI_PARAMS
	//// Group all together
	//params_MIDI.clear();
	//params_MIDI.add(params_Preset); // -> The ofParams of each Preset 
	//params_MIDI.add(params_PresetToggles); // -> To select the index preset by note/toggle (exclusive)
	//params_MIDI.add(index); // -> Add an int to select the index preset
	//surfingMIDI.add(params_MIDI);

	// Splitted
	//surfingMIDI.clear(); //- This clears all settings!
	surfingMIDI.add(params_Preset); // -> The ofParams of each Preset 
	surfingMIDI.add(params_PresetToggles); // -> To select the index preset by note/toggle (exclusive)
	surfingMIDI.add(index); // -> Add an int to select the index preset
	surfingMIDI.add(surfingPlayer.bPlay); // -> Add an int to select the index preset

	//surfingMIDI.connect();
	//surfingMIDI.startup();
#endif

	//-

#ifdef INCLUDE__OFX_SURFING_PRESET__OFX_PARAMETER_MIDI_SYNC
	// Group all together
	params_MIDI.clear();
	params_MIDI.add(params_Preset); // -> The ofParams of each Preset 
	params_MIDI.add(params_PresetToggles); // -> To select the index preset by note/toggle (exclusive)
	params_MIDI.add(index); // -> Add an int to select the index preset

	surfingMIDI.setup(params_MIDI); // -> The ofParams of each Preset 
#endif
}

#endif

//--------------------------------------------------------------
void ofxSurfingPresets::startup()
{
	ofLogNotice(__FUNCTION__);

	//--

	// MIDI
	{
		//--

		// Toggles 
		// Prepare. Exclusive for index selector

#ifdef USE_TOGGLE_TRIGGERS

		notesIndex.clear();
		params_PresetToggles.clear();

		for (int i = 0; i <= index.getMax(); i++)
		{
			std::string n = "Preset ";
			//n += ofToString(i < 10 ? "0" : "");
			n += ofToString(i);

			ofParameter<bool> b{ n, false };
			notesIndex.push_back(b);
			params_PresetToggles.add(b);
		}

		ofAddListener(params_PresetToggles.parameterChangedE(), this, &ofxSurfingPresets::Changed_Params_PresetToggles);
#endif

		//--

		// Initiates

#ifdef INCLUDE__OFX_SURFING_PRESET__OFX_MIDI_PARAMS
		surfingMIDI.connect();
#endif

#ifdef INCLUDE__OFX_SURFING_PRESET__OFX_PARAMETER_MIDI_SYNC
#endif

#ifdef USE__OFX_SURFING_PRESET__MIDI__
		doRecreateMidi();
#endif

	}

	//--

	// Player

#ifdef USE__OFX_SURFING_PRESETS__OFX_SURFING_PLAYER

	if (!bDisablePlayer)
	{
		// Moved here for avoid setup() crashes bd initialization not finished!

		// Define the behaviors to trig when player trigs the callback!
		//--------------------------------------------------------------
		listener_Beat = playerSurfer.bPlayerBeatBang.newListener([this](bool& b)
			{
				ofLogNotice("BEAT: ") << (b ? "TRUE" : "FALSE");
				if (!b) return;

				// bangs can be bypassed when not playing
				//if (playerSurfer.bPlay) 
				{
					switch (randomTypePlayIndex)
					{
					case 0: doLoadNext(); break;
					case 1: doRandomizeIndex(); break;
					case 2: doRandomizeParams(); break;
					}
				}
			});

		//--------------------------------------------------------------
		listener_Play = playerSurfer.bPlay.newListener([this](bool& b)
			{
				ofLogNotice("PLAY: ") << (b ? "TRUE" : "FALSE");

				// workflow
				// disable edit mode when playing to avoid accidental overwrite settings
				// also could improve performance reducing saving files!
				if (!b) return;
				else
				{
					if (bEditMode) bEditMode = false;
				}
			});
	}

#endif

	//-

	// Remote
	// Server

#ifdef USE__OFX_SURFING_CONTROL__OFX_REMOTE_PARAMETERS__SERVER

	// Group all together
	params_Server.clear();
	params_Server.add(index); // -> Add an int to select the index preset
	params_Server.add(params_PresetToggles); // -> To select the index preset by note/toggle (exclusive)
	params_Server.add(params_Preset); // -> The ofParams of each Preset 

	remoteServer.setup(params_Server);

	// listen local changes to update connected client remote app!
	ofAddListener(params_Preset.parameterChangedE(), this, &ofxSurfingPresets::Changed_Params_Preset);

#endif

	//-

	bDISABLE_CALLBACKS = false;

	//--

	// Force default states
	if (0)
	{
		bGui_Parameters = false;
		bGui_Editor = false;

		//ui.getWindowsSpecialsGuiToggle().set(false);
		//ui.getWindowsAlignHelpersGuiToggle().set(false);
	}

	//--

	// Settings
	bNoSettingsFileFound = !ofxSurfingHelpers::loadGroup(params_AppSettings, path_Global + path_Params_Control);
	// Notice that Will return true if no settings files found: first time that we open the app! 

	// Path for settings
	ofxSurfingHelpers::CheckFolder(path_Global);
	ofxSurfingHelpers::CheckFolder(path_Presets);

	//-

	// List data files
	bool b = doRefreshFiles();

	//TODO:
	// sometimes fails and populate more presets when dir is not empty!
	// Initiate
	if (!b) {
		doPopulatePresetsRandomized(); // if no files then populate a bundle of randomized presets.

		//-

		//// Create first preset if folder it's empty
		//if (dir.size() == 0)
		//{
		//	doNewPreset();
		//}

		//--

		doRefreshFiles();
	}

	//--

	// if no settings yet, set a default 3 presets per row.
	// NOTICE that usually use it for 3 rows of 3 presets matrices. with a total of 9 presets!
	if ((amountButtonsPerRowClicker == -1) || (amountButtonsPerRowClickerMini == -1)) {
		setAmountButtonsPerRowClickers(3);
	}

	// Workflow

	// Force Load first preset
	//index = 0;

	buildHelp();
}

//--------------------------------------------------------------
void ofxSurfingPresets::update(ofEventArgs& args)
{
	// auto reload preset when clicked. that's to undo current editing!
	if (bAutoLoadOnReTrig)
		if (isRetrigged())
		{
			doReload();
		}

	//--

	//// Align Windows Engine

	//if (ui.bLinkWindows)
	//{
	//	if (ofGetFrameNum() == 2)
	//	{
	//		if (bNoSettingsFileFound) doAlignWindowsOnce();
	//		// First time opening! We align windows.
	//	}
	//}
	//// Workflow
	//if (bGui_Changed)
	//{
	//	doAlignWindowsOnce();

	//	// works weird..
	//	// Workaround
	//	// Repeat x frames bc ImGui must be refreshed...
	//	countTimes--;
	//	if (countTimes == 0) bGui_Changed = false;
	//}

	//--


	/*

	// Auto save

	if (bAutoSaveTimed && ofGetElapsedTimeMillis() - timerLast_Autosave > timeToAutosave)
	{
		bDISABLE_CALLBACKS = true;

		ofxSurfingHelpers::saveGroup(params_AppSettings, path_Global + path_Params_Control);

		doSaveCurrent();

		bDISABLE_CALLBACKS = false;

		timerLast_Autosave = ofGetElapsedTimeMillis();
		ofLogNotice(__FUNCTION__) << "Autosaved DONE";
	}

	*/

	//--

#ifdef USE__OFX_SURFING_CONTROL__OFX_REMOTE_PARAMETERS__SERVER
	if (bSyncRemote) {
		bSyncRemote = false;
		//// Note that if you use the GUI the client does not update automatically. If you want the client to update
		//// you will need to call paramServer.syncParameters() whenever a parameter does change.
		remoteServer.syncParameters();
	}
#endif

	//--

#ifdef USE__OFX_SURFING_PRESETS__BASIC_SMOOTHER
	updateSmoother();
#endif
}

//--------------------------------------------------------------
void ofxSurfingPresets::draw()
{
	if (!bGui_Global) return;
	if (!bGui) return;

	//TODO: should be done manually to avoid some locking trouble when multi instances
	draw_ImGui();

	//-

#ifdef INCLUDE__OFX_SURFING_PRESET__OFX_MIDI_PARAMS
	surfingMIDI.draw();
#endif

#ifdef INCLUDE__OFX_SURFING_PRESET__OFX_PARAMETER_MIDI_SYNC
	surfingMIDI.drawImGui();
#endif

	if (ui.bHelpInternal)
	{
		static bool bKeys_PRE = false;
		if (bKeys != bKeys_PRE) {
			bKeys_PRE = bKeys;
			buildHelp();
		}

		textBoxWidget.draw();
	}
}

//--

//TODO:
#ifdef USE__OFX_SURFING_PRESETS__BASIC_SMOOTHER
//--------------------------------------------------------------
void ofxSurfingPresets::updateSmoother()
{
	if (!bSmooth) return;

	//-

	// Smoothed
	{
		//-

		// Map Speed to tweak limits

		//MAX_CLAMP_SMOOTH = 0.85f;
		sp = ofMap(smoothSpeed, 1, 0, 0.45f, MAX_CLAMP_SMOOTH);

		//-

		auto& g = params_Preset_Smoothed.castGroup();

		for (int i = 0; i < g.size(); i++)
		{
			auto& ap = g[i]; // ofAbstractParameter

			updateSmoothParam(ap);
		}
	}
}

//TODO: fix crash!
//crashes sometimes maybe depending on scope "types" of the classess that is adding params...
//--------------------------------------------------------------
void ofxSurfingPresets::updateSmoothParam(ofAbstractParameter& ap)
{
	auto type = ap.type();
	auto name = ap.getName();

	bool isFloat = type == typeid(ofParameter<float>).name();
	bool isInt = type == typeid(ofParameter<int>).name();
	bool isGroup = type == typeid(ofParameterGroup).name();

	if (!isFloat && !isInt && !isGroup) return;

	//TODO:
	// add more types. multidim, colors

	if (0) {}

	// Float
	else if (isFloat)
	{
		auto pVal = ap.cast<float>();
		auto pTar = params_Preset.getFloat(name);//crashes here

		//ofParameter<float> pVal = ap.cast<float>();
		//ofParameter<float> pTar = params_Preset.getFloat(name);

		float v = pVal.get();
		ofxSurfingHelpers::ofxKuValueSmooth(v, pTar.get(), sp);
		pVal = v;
	}

	// Int
	else if (isInt)
	{
		ofParameter<int> pVal = ap.cast<int>();
		ofParameter<int> pTar = params_Preset.getInt(name); ;
		int v = pVal.get();
		ofxSurfingHelpers::ofxKuValueSmooth(v, pTar.get(), sp);
		pVal = v;
	}

	// Group
	else if (isGroup)
	{
		auto& g = ap.castGroup();

		// ofAbstractParameters
		for (int i = 0; i < g.size(); i++)
		{
			updateSmoothParam(g.get(i));
		}
	}

	else
	{
		//ofLogError(__FUNCTION__) << "Not the expected type for ofParam: " << name;
	}
}

#endif

//--

//--------------------------------------------------------------
void ofxSurfingPresets::draw_ImGui_Editor()
{
	if (!bGui_Editor) return;

	// Widgets sizes
	float _w1;
	float _w2;
	float _h;

	//-

	// 1. Editor
	{
		//if (bGui_Editor) ImGui::PushID(("##EDIT" + params_Preset.getName()).c_str());
		{
			IMGUI_SUGAR__WINDOWS_CONSTRAINTSW_SMALL;

			if (ui.BeginWindowSpecial(bGui_Editor))
			{
				ui.AddLabelBig(bGui_Editor.getName(), false);

				//--

				_w1 = getWidgetsWidth(1);
				_w2 = getWidgetsWidth(2);
				_h = getWidgetsHeightUnit();

				//--

				// Minimize

				ui.Add(bMinimize_Editor, OFX_IM_TOGGLE_BUTTON_ROUNDED);

				//--

				ui.AddSpacingSeparated();

				//--

				if (!bMinimize_Editor)
				{
					ui.AddLabelBig("Panels", true, true);
				}

				// Player

#ifdef USE__OFX_SURFING_PRESETS__OFX_SURFING_PLAYER 
				if (!bDisablePlayer)
				{
					ui.Add(playerSurfer.bGui, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
				}
#endif
				//--

				// Undo Engine

#ifdef USE__OFX_SURFING__OFX_SURFING_UNDO_HELPER 
				{
					ui.Add(undoManager.bGui_UndoEngine, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
				}
#endif

				//--

				//if (!bMinimize_Editor)
				{
					//--

					// Parameters

					ui.Add(bGui_Parameters, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);

					//ui.AddSpacingSeparated();
				}

				//--

				// Scrollable list

				if (!fileNames.empty())
				{
					ui.AddSpacingSeparated();
					ui.AddSpacing();

					int _i = index;
					if (ofxImGuiSurfing::VectorCombo(" ", &_i, fileNames))
					{
						ofLogNotice(__FUNCTION__) << "_i: " << ofToString(_i);

						if (_i < fileNames.size())
						{
							index = _i;
						}
					}
				}

				// Index
				//TODO: starts on zero... should correlate to clicker starting at 1
				// //or displaying each key command on the matrix clicker!
				//ui.Add(index);

				// Label

				if (!bMinimize_Editor)
				{
					std::string ss;
					if (dir.size() == 0) ss = "NO PRESETS";
					else ss = ofToString(index) + "/" + ofToString(index.getMax());
					ui.AddSpacing();
					ImGui::Text(ss.data());
				}

				// Browse

				if (bMinimize_Editor)
				{
					ImGui::PushButtonRepeat(true);
					{
						if (ImGui::Button("<", ImVec2(_w2, _h * 1.25f)))
						{
							doLoadPrevious();
						}

						ImGui::SameLine();

						if (ImGui::Button(">", ImVec2(_w2, _h * 1.25f)))
						{
							doLoadNext();
						}
					}
					ImGui::PopButtonRepeat();
				}

				//--

				// Simple Clicker 

				// Inner and simple, 
				// non floating clicker to embed into GUI contents.

				if (!bMinimize_Editor)
					if (bGui_ClickerSimple)
					{
						ui.AddSpacingSeparated();
						ui.AddLabelBig("Clicker Simple");
						draw_ImGui_ClickerSimple(false, true, false);
					}

				//--

				if (!bMinimize_Editor)
				{
					ui.AddSpacing();

					ImGui::PushButtonRepeat(true);
					{
						if (ImGui::Button("<", ImVec2(_w2, _h * 1.25f)))
						{
							doLoadPrevious();
						}
						ImGui::SameLine();
						if (ImGui::Button(">", ImVec2(_w2, _h * 1.25f)))
						{
							doLoadNext();
						}
					}
					ImGui::PopButtonRepeat();

					// Save, Load
					ui.Add(bSave, OFX_IM_BUTTON, 2, true);
					ui.Add(bLoad, OFX_IM_BUTTON, 2, false);

					// New
					ui.Add(bNewPreset, OFX_IM_BUTTON, 2, true);
					ui.AddTooltip("Create a new Preset at the end");

					// Delete
					if (ImGui::Button("DELETE", ImVec2(_w2, _h * 1.25f))) ImGui::OpenPopup("DELETE ?");
					if (ImGui::BeginPopupModal("DELETE ?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
					{
						ui.AddLabelBig("Current Preset \nwill be deleted.", true, true);
						ui.AddSpacing();
						ui.AddLabelBig("This operation \ncannot be undone!", true, true);
						ui.AddSpacingBig();

						static bool dont_ask_me_next_time = false;
						ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
						ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
						ImGui::PopStyleVar();

						ui.AddSpacing();

						if (!dont_ask_me_next_time) {
							if (ImGui::Button("OK", ImVec2(120, 2 * _h))) {
								ofLogNotice(__FUNCTION__) << "DELETE";
								doDeletePreset(index);
								ImGui::CloseCurrentPopup();
							}
							ImGui::SetItemDefaultFocus();
							ImGui::SameLine();
							if (ImGui::Button("Cancel", ImVec2(120, 2 * _h))) {
								ImGui::CloseCurrentPopup();
							}
						}
						else {
							ofLogNotice(__FUNCTION__) << "DELETE";
							doDeletePreset(index);
							ImGui::CloseCurrentPopup();
						}

						ImGui::EndPopup();
					}
					ui.AddTooltip("Remove current Preset");

					ui.AddSpacingSeparated();
				}

				//--

				// Presets Tools

				if (bMinimize_Editor) ui.AddSpacingSeparated();

				ui.AddLabel("Tools", true, true);
				draw_ImGui_ToolsPreset(false);

				//--

				// Undo Manager

#ifdef USE__OFX_SURFING__OFX_SURFING_UNDO_HELPER 
				ui.AddSpacingSeparated();

				// Browse
				ui.AddLabel("Undo History", true, true);
				// Undo / Redo
				undoManager.drawImGuiWidgetsBrowse(true);

				// History
				undoManager.drawImGuiWidgetsHistoryInfo(true);
#endif

				//--

				if (!bMinimize_Editor) ui.AddSpacingSeparated();

				//--

				// MIDI

#ifdef USE__OFX_SURFING_PRESET__MIDI__
				ui.Add(surfingMIDI.bGui, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
#endif
				//--

				if (!bMinimize_Editor)
				{
					bool bOpen = false;
					ImGuiTreeNodeFlags _flagt = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
					_flagt |= ImGuiTreeNodeFlags_Framed;

					if (ImGui::TreeNodeEx("KIT", _flagt))
					{
						draw_ImGui_ToolsKit();

						ImGui::TreePop();
					}
				}

				//-

				// Extra

				static ofParameter<bool> bFiles{ "Files", false };

				if (!bMinimize_Editor)
				{
					ui.AddSpacingSeparated();

					ui.Add(ui.bExtra, OFX_IM_TOGGLE_BUTTON_ROUNDED);

					if (ui.bExtra)
					{
						ui.Indent();
						{
							// Organizer Aligners
							ui.Add(ui.bGui_Organizer, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
							ui.AddTooltip("Panel to align, link, cascade or reset windows");

							ui.AddSpacing();

							//--

							// Smooth

							// Extra app

							if (params_AppExtra.getName() != "-1")
							{
								static ofParameter<bool> b = params_AppExtra.getBool("Smooth");
								static ofParameter<float> v = params_AppExtra.getFloat("Speed");

								ui.Add(b, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
								if (b)
								{
									ui.Indent();
									ofxImGuiSurfing::AddParameter(v, "%.1f");
									//ui.Add(v, OFX_IM_HSLIDER_SMALL_NO_LABELS, 2);
									ui.Unindent();
								}
							}

							ui.Add(bGui_ClickerSimple, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
							if (bGui_ClickerSimple)
							{
								ui.Indent();
								static bool bOpen = false;
								ImGuiColorEditFlags _flagw = (bOpen ? ImGuiWindowFlags_NoCollapse : ImGuiWindowFlags_None);
								if (ImGui::CollapsingHeader("CLICKER SIMPLE", _flagw))
								{
									ui.Add(bResponsiveButtonsClickerSimple, OFX_IM_CHECKBOX);
									if (bResponsiveButtonsClickerSimple)
									{
										ui.Add(amountButtonsPerRowClickerMini, OFX_IM_STEPPER);
									}
								}
								ui.Unindent();
							}

							ui.Add(bCycled, OFX_IM_CHECKBOX);
							ui.AddTooltip("Allows browsing by arrows unlocked");

							//-

							// Files

							ui.Add(bFiles, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
							if (bFiles)
							{
								ui.Indent();
								{
									// Paths
									{
										bool bOpen = false;
										ImGuiTreeNodeFlags _flagt = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
										//_flagt |= ImGuiTreeNodeFlags_Framed;

										if (ImGui::TreeNodeEx("Path", _flagt))
										{
											ImGui::TextWrapped(path_Presets.data()); // -> show path
											ImGui::TreePop();
										}
									}

									// Files
									// Buttons Selector for each file
									if (ofxImGuiSurfing::filesPicker(path_Presets, nameSelected, index, { "json" }))
									{
										// Buttons Matrix

										//TODO: 
										// Index back not working
										// this is a workaround
										// could fail on macOS/Linux -> requires fix paths slashes

										for (int i = 0; i < dir.size(); i++)
										{
											std::string si = ofToString(i);
											if (i < 10) si = "0" + si;
											std::string ss = name_Root + "_" + si;
											fileName = ss;

											auto s0 = ofSplitString(nameSelected, "\\", true);
											std::string s1 = s0[s0.size() - 1]; // filename
											auto s = ofSplitString(s1, ".json");

											std::string _nameSelected = s[0];

											if (_nameSelected == fileName)
											{
												index = i;
											}
										}

										ofLogNotice(__FUNCTION__) << "Picked file " << nameSelected << " > " << index;
									}
								}
								ui.Unindent();
							}
						}
						ui.Unindent();

					}
				}

				//-

				ui.EndWindowSpecial();
				//ui.EndWindowSpecial(bGui_Editor);
			}
		}
		//if (bGui_Editor) ImGui::PopID();
	}
}

//--------------------------------------------------------------
void ofxSurfingPresets::draw_ImGui_Main()
{
	if (!bGui) return;

	IMGUI_SUGAR__WINDOWS_CONSTRAINTSW_SMALL;

	// Widgets Sizes
	float _w1;
	float _w2;
	float _w3;
	float _w4;
	float _h;

	// avoid collision with other presets manager instances
	// if the windows have the same name.
	//if (bGui) ImGui::PushID(("##MAIN" + params_Preset.getName()).c_str());
	{
		if (ui.BeginWindowSpecial(bGui))
		{
			string n = "PRESETS \n" + params_Preset.getName();
			ui.AddLabelBig(n, false);
			ui.AddSpacing();

			//--

			_w1 = getWidgetsWidth(1);
			_w2 = getWidgetsWidth(2);
			_w3 = getWidgetsWidth(3);
			_h = getWidgetsHeightUnit();

			//--

			// Minimize
			ui.Add(bMinimize, OFX_IM_TOGGLE_BUTTON_ROUNDED);

			// Keys
			if (!bMinimize) ui.Add(bKeys, OFX_IM_TOGGLE_BUTTON_ROUNDED);

			//ui.AddSpacing();
			ui.AddSpacingSeparated();

			bool b = true;
			if (bMinimize) {
				b = ui.BeginTree("Panels");
			}

			if (b)
			{
				// Editor
				ui.Add(bGui_Editor, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);

				// Player
				if (!bGui_Editor)
					if (!bDisablePlayer)
					{
						ui.Add(playerSurfer.bGui, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
					}
			}

			if (bMinimize) {
				if (b) ui.EndTree();
			}

			//----

			ui.AddSpacingSeparated();

			// debug
			if (0) {
				ui.Add(index);
				ui.AddToggle("Flip", bFlip);
				ui.AddSpacingSeparated();
			}

			//--

			// Clicker Matrix
			{
				float _h2 = 2 * ui.getWidgetsHeightUnit();

				string toolTip = "";
				if (bKeyCtrl) toolTip = "Copy To";
				else if (bKeyAlt) toolTip = "Swap With";
				ofxImGuiSurfing::AddMatrixClickerLabels(index, keyCommandsChars, bResponsiveButtonsClicker, amountButtonsPerRowClicker, true, _h2, toolTip, bFlip);

				//TODO:
				// using Ptr
				//ofxImGuiSurfing::AddMatrixClickerLabels(index, (char *) keyCommandsChars, bResponsiveButtonsClicker, amountButtonsPerRowClicker, true, _h2);
			}

			ui.AddSpacingSeparated();
			//ui.AddSpacing();

			//--

			// Edit mode

			ui.Add(bEditMode, OFX_IM_TOGGLE_BIG_BORDER_BLINK);
			//ui.Add(bEditMode, bMinimize ? OFX_IM_TOGGLE_BORDER_BLINK : OFX_IM_TOGGLE_BIG_BORDER_BLINK);
			if (bEditMode) ui.AddTooltip("Auto Save when modified parameters.");
			else ui.AddTooltip("Requires manual Save!");

			// Save, Load
			if (!bMinimize || (!bEditMode && bMinimize))
			{
				ui.AddSpacing();
				ui.Add(bSave, bEditMode ? OFX_IM_BUTTON : OFX_IM_BUTTON_BORDER_BLINK, 2, true);
				ui.Add(bLoad, OFX_IM_BUTTON, 2, false);
				ui.AddTooltip("Reload last Preset. Discard last modifications.");
			}

			//--

			// Play

#ifdef USE__OFX_SURFING_PRESETS__OFX_SURFING_PLAYER 

			if (!bDisablePlayer)
			{
				if (!(bMinimize && playerSurfer.bGui))
				{
					ui.AddSpacingSeparated();

					//_h = 3 * getWidgetsHeightUnit();
					_h = (bMinimize ? 2 : 3) * getWidgetsHeightUnit();
					_w1 = getWidgetsWidth(1);

					ofxImGuiSurfing::AddBigToggleNamed(playerSurfer.bPlay,
						_w1, _h, "PLAYING", "PLAY", true, playerSurfer.getPlayerProgress());

					if (!bMinimize)
					{
						// Target
						ofxImGuiSurfing::AddCombo(randomTypePlayIndex, randomTypesPlayNames);

						// Bang
						ui.Add(playerSurfer.bPlayerBeatBang, OFX_IM_BUTTON);
					}
				}
			}
#endif
			//--

#ifdef USE__OFX_SURFING__OFX_SURFING_UNDO_HELPER
			if (!bGui_Editor)
			{
				ui.AddSpacingSeparated();

				// Browse
				// Undo / Redo
				undoManager.drawImGuiWidgetsBrowse(true);

				// History
				undoManager.drawImGuiWidgetsHistoryInfo(true);
			}
#endif
			//--

			if (!bMinimize)
			{
				ui.AddSpacingSeparated();

				//--

				// MIDI

#ifdef USE__OFX_SURFING_PRESET__MIDI__
				ui.Add(surfingMIDI.bGui, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
#endif
				//--

				// Extra
				ui.Add(bExtra_Main, OFX_IM_TOGGLE_BUTTON_ROUNDED);
			}

			//--

			// Extra

			if (!bMinimize)
				if (bExtra_Main)
				{
					//ui.AddSpacingBigSeparated();

					ui.Indent();
					{
						_w1 = getWidgetsWidth(1);
						_w2 = getWidgetsWidth(2);

						//--

						// Organizer Aligners
						ui.Add(ui.bGui_Organizer, OFX_IM_TOGGLE_BUTTON_ROUNDED);
						ui.AddSpacing();

						//--

						{
							static bool bOpen = false;
							ImGuiColorEditFlags _flagw = (bOpen ? ImGuiWindowFlags_NoCollapse : ImGuiWindowFlags_None);
							if (ImGui::CollapsingHeader("CLICKER", _flagw))
							{
								ui.refreshLayout();
								if (bResponsiveButtonsClicker)
								{
									ui.Add(amountButtonsPerRowClicker, OFX_IM_STEPPER);
								}

								ui.Add(bResponsiveButtonsClicker, OFX_IM_CHECKBOX);
								ui.Add(bAutoResize_Clicker, OFX_IM_CHECKBOX);
							}
						}
					}
					ui.Unindent();
				}

			// Help
			if (!bMinimize) {
				ui.AddSpacingSeparated();
				ui.Add(ui.bHelpInternal, OFX_IM_TOGGLE_BUTTON_ROUNDED);
			}

			ui.EndWindowSpecial();
		}
	}
	//if (bGui) ImGui::PopID();
}

//--------------------------------------------------------------
void ofxSurfingPresets::draw_ImGui_ClickerSimple()
{
	if (!bGui_ClickerSimple) bGui_ClickerSimple = true;//workaround..

	draw_ImGui_ClickerSimple(true, false, true, true);
}

/*
//snippet
bool bHeader = true;
bool bMinimal = true;
bool bShowMinimize = true;
bool bNoExtras = false;
*/

//--------------------------------------------------------------
void ofxSurfingPresets::draw_ImGui_ClickerSimple(bool bHeader, bool bMinimal, bool bShowMinimize, bool bNoExtras)
{
	if (!bGui_ClickerSimple) return;

	static bool bOpen = !bHeader;
	ImGuiTreeNodeFlags _flagt = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
	_flagt |= ImGuiTreeNodeFlags_Framed;

	float _hm = ui.getWidgetsHeightUnit();
	_hm *= ui.bMinimize ? 1.2f : 1.5f;

	bool b;
	if (bHeader) b = (ImGui::TreeNodeEx("PRESETS", _flagt));
	else b = true;

	if (b)
	{
		ui.refreshLayout();

		if (bShowMinimize) ui.Add(ui.bMinimize, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);

		//--

		// Index

		if (!ui.bMinimize) if (!bNoExtras) ui.Add(index);

		//--

		// Clicker

		ofxImGuiSurfing::AddMatrixClickerLabels(index, keyCommandsChars, bResponsiveButtonsClickerSimple, amountButtonsPerRowClickerMini, true, _hm);

		ui.AddSpacing();

		if (!bNoExtras)
		{

#ifdef USE__OFX_SURFING_PRESETS__OFX_SURFING_PLAYER 
			if (!bDisablePlayer && !bMinimal)
			{
				float _h = getWidgetsHeightUnit();
				float _r = ui.bMinimize ? 1.5f : 2.0f;
				float _w1 = getWidgetsWidth(1);

				ofxImGuiSurfing::AddBigToggleNamed(playerSurfer.bPlay,
					_w1, _r * _h, "PLAYING", "PLAY", true, playerSurfer.getPlayerProgress());
			}
#endif

			if (!ui.bMinimize && !bMinimal)
			{
				ImGui::PushID("##CLICKEREDIT");
				//TODO: Bug ??
				//ui.Add(bEditMode, OFX_IM_TOGGLE);
				//ui.Add(bEditMode, OFX_IM_TOGGLE_BIG_XXL);
				ui.Add(bEditMode, OFX_IM_TOGGLE_ROUNDED);
				ImGui::PopID();

				if (!bEditMode.get()) ui.Add(bSave, OFX_IM_BUTTON);
			}
		}
	}

	if (bHeader && b) ImGui::TreePop();
}

//--------------------------------------------------------------
void ofxSurfingPresets::draw_ImGui_ClickerMinimal()
{
	bool bOpen = false;
	ImGuiTreeNodeFlags _flagt = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
	_flagt |= ImGuiTreeNodeFlags_Framed;

	bool bHeader = false;
	bool b = true;

	if (bHeader) b = ImGui::TreeNodeEx("PRESETS", _flagt);

	if (b)
	{
		float _w1 = ofxImGuiSurfing::getWidgetsWidth(1);
		float _w2 = ofxImGuiSurfing::getWidgetsWidth(2);
		float _h = getWidgetsHeightRelative();

		// Scrollable list

		if (!fileNames.empty())
		{
			ui.AddCombo(index, fileNames);
		}

		// Minimize
		ImGui::PushID("#CLICKERMINI");
		ui.Add(ui.bMinimize, OFX_IM_TOGGLE_BUTTON_ROUNDED);
		ImGui::PopID();

		// Index

		if (!ui.bMinimize)
		{
			ofxImGuiSurfing::AddParameter(index);
		}

		if (!ui.bMinimize)
		{
			ImGui::PushButtonRepeat(true);
			{
				if (ImGui::Button("<", ImVec2(_w2, _h)))
				{
					doLoadPrevious();
				}
				ImGui::SameLine();
				if (ImGui::Button(">", ImVec2(_w2, _h)))
				{
					doLoadNext();
				}
			}
			ImGui::PopButtonRepeat();
		}

		if (!ui.bMinimize)
		{
			ofxImGuiSurfing::AddToggleRoundedButton(bGui);
		}

		if (bHeader) ImGui::TreePop();
	}
}

//--------------------------------------------------------------
void ofxSurfingPresets::draw_ImGui_Parameters()
{
	// Parameters

	if (bGui_Parameters)
	{
		if (params_Preset.getName() != "-1")
		{
			//if (bGui_Parameters) ImGui::PushID(("##PARAMS" + params_Preset.getName()).c_str());
			{
				if (ui.BeginWindowSpecial(bGui_Parameters))
				{
					ui.AddLabelBig(bGui_Parameters.getName(), false);

					ui.AddGroup(params_Preset);

					ui.EndWindowSpecial();
				}
			}
			//if (bGui_Parameters) ImGui::PopID();
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingPresets::draw_ImGui_ToolsKit()
{
	ui.refreshLayout();
	float _w1 = getWidgetsWidth(1);
	float _h = getWidgetsHeightUnit();
	_h *= 1.25f;

	{
		if (ImGui::Button("CLEAR", ImVec2(_w1, _h)))
		{
			ImGui::OpenPopup("CLEAR KIT ?");
		}
		ui.AddTooltip("Remove all file Presets!");

		if (ImGui::BeginPopupModal("CLEAR KIT ?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ui.AddLabelBig("Presets Kit \nwill be erased.", true, true);
			ui.AddSpacing();
			ui.AddLabelBig("This operation \ncannot be undone!", true, true);
			ui.AddSpacingBig();

			static bool dont_ask_me_next_time = false;
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
			ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
			ImGui::PopStyleVar();

			ui.AddSpacing();

			if (!dont_ask_me_next_time) {
				if (ImGui::Button("OK", ImVec2(120, 2 * _h))) {
					ofLogNotice(__FUNCTION__) << "CLEAR";
					doClearPresets();
					ImGui::CloseCurrentPopup();
				}
				ImGui::SetItemDefaultFocus();
				ImGui::SameLine();
				if (ImGui::Button("Cancel", ImVec2(120, 2 * _h))) {
					ImGui::CloseCurrentPopup();
				}
			}
			else {
				ofLogNotice(__FUNCTION__) << "CLEAR";
				doClearPresets();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		if (ImGui::Button("POPULATE", ImVec2(_w1, _h)))
		{
			doPopulatePresets();
		}
		ui.AddTooltip("Clear Kit and create New Presets copying current");

		if (ImGui::Button("POPULATE RAND", ImVec2(_w1, _h)))
		{
			doPopulatePresetsRandomized();
		}
		ui.AddTooltip("Clear Kit and create New randomized Presets");
		if (ui.bExtra)
		{
			if (ImGui::Button("RECREATE", ImVec2(_w1, _h)))
			{
				doRefreshFilesAndRename();
			}
			ui.AddTooltip("Fix file names and reload Presets");

			ui.Add(bRefresh, OFX_IM_BUTTON_SMALL);
			ui.AddTooltip("Reload Preset files");

			ui.Add(bSetPathPresets, OFX_IM_BUTTON_SMALL);
			ui.AddTooltip("Open dialog to customize Presets folder");
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingPresets::draw_ImGui_ToolsPreset(bool bMini)
{
	float _w1 = ofxImGuiSurfing::getWidgetsWidth(1);
	float _w2 = ofxImGuiSurfing::getWidgetsWidth(2);
	float _h = getWidgetsHeightUnit();

	if (!bMini) _h *= 1.25f;

	if (ImGui::Button("RESET", ImVec2(_w2, _h)))
	{
		doResetParams();
	}
	ui.AddTooltip("Reset current Preset parameters to min values");

	ImGui::SameLine();

	if (ImGui::Button("RANDOM", ImVec2(_w2, _h)))
	{
		doRandomizeParams();
	}
	ui.AddTooltip("Randomize current Preset parameters");

	//--

	if (ImGui::Button("STORE", ImVec2(_w2, _h)))
	{
		doStoreState();
	}
	ui.AddTooltip("Store current Preset State");

	ImGui::SameLine();

	if (ImGui::Button("RECALL", ImVec2(_w2, _h)))
	{
		doRecallState();
	}
	ui.AddTooltip("Recall current Preset stored State");

}

//--------------------------------------------------------------
void ofxSurfingPresets::draw_ImGui()
{
	if (!bGui_Global) return;

	ui.Begin();
	{
		draw_ImGui_Main();

		//--

		draw_ImGui_Editor();

		//--

		//workflow
		//if (bGui_Editor) draw_ImGui_Parameters();
		draw_ImGui_Parameters();

		//--

		// Undo Engine
#ifdef USE__OFX_SURFING__OFX_SURFING_UNDO_HELPER 

		//workflow
		//if (bGui_Editor) undoManager.drawImGuiWindow();
		undoManager.drawImGuiWindow();
#endif

	}
	ui.End();

	//--

	// Player

#ifdef USE__OFX_SURFING_PRESETS__OFX_SURFING_PLAYER
	if (!bDisablePlayer)
	{
		if (playerSurfer.bGui)
		{
			//ui.setNextWindowOnViewport();

			//--

			//TODO: fix
			//string n = ui.getWindowSpecialLast();

			////string n2 = "PLAYER";
			//string n2 = playerSurfer.bGui.getName();

			//cout << n << "  -  " << n2 << endl;

			//if (n != "-1" && n != n2)
			//{
			//	ui.setNextWindowAfterWindowNamed(n);
			//}

			//--

			playerSurfer.draw();
		}
	}
#endif

	//--
}

//--------------------------------------------------------------
void ofxSurfingPresets::keyPressed(ofKeyEventArgs& eventArgs)
{
	const int& key = eventArgs.key;
	ofLogNotice(__FUNCTION__) << (char)key << " [" << key << "]";

	// Modifiers
	bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
	bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
	bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
	bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);

	if (mod_CONTROL) bKeyCtrl = true;
	if (mod_ALT) bKeyAlt = true;

	//--

	if (!bKeys) return;

	//--

	// Undo Engine

#ifdef USE__OFX_SURFING__OFX_SURFING_UNDO_HELPER 
	undoManager.keyPressed(eventArgs);
#endif

	//--

	if (0)
	{
		ofLogNotice(__FUNCTION__) << "mod_COMMAND : " << (mod_COMMAND ? "ON" : "OFF");
		ofLogNotice(__FUNCTION__) << "mod_CONTROL : " << (mod_CONTROL ? "ON" : "OFF");
		ofLogNotice(__FUNCTION__) << "mod_ALT     : " << (mod_ALT ? "ON" : "OFF");
		ofLogNotice(__FUNCTION__) << "mod_SHIFT   : " << (mod_SHIFT ? "ON" : "OFF");
	}

	if (0) {}

	else if (key == 'G') {
		setGuiVisibleToggle();
		return;
	}

	else if (key == OF_KEY_LEFT) {
		if (bKeysArrows) doLoadPrevious();
		return;
	}
	else if (key == OF_KEY_RIGHT) {
		if (bKeysArrows) doLoadNext();
		return;
	}
	if (!bKeyCtrl && key == ' ') {
		if (bKeySpace) doLoadNext();
		return;
	}

	//--

	// Player

#ifdef USE__OFX_SURFING_PRESETS__OFX_SURFING_PLAYER 

	else if (bKeyCtrl && key == ' ')
	{
		if (!bDisablePlayer)
		{
			playerSurfer.setPlayToggle();
		}
		return;
	}

#endif

	//--

	else if (key == OF_KEY_BACKSPACE) {
		doResetParams();
		return;
	}
	else if (key == OF_KEY_RETURN) {
		doRandomizeParams();
		return;
	}

	else if (key == 'S') {
		doStoreState();
		return;
	}
	else if (key == 'R') {
		doRecallState();
		return;
	}

	//--

	// Preset Index Selector by key numbers

	else
		for (int i = 0; i < keyCommandsChars.size(); i++) {
			if (key == keyCommandsChars[i]) {
				load(i);
				return;
			}
		}
}

//--------------------------------------------------------------
void ofxSurfingPresets::keyReleased(ofKeyEventArgs& eventArgs)
{
	const int& key = eventArgs.key;
	ofLogNotice(__FUNCTION__) << (char)key << " [" << key << "]";

	// Modifiers
	bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
	bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
	bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
	bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);

	if (!mod_CONTROL) bKeyCtrl = false;
	if (!mod_ALT) bKeyAlt = false;

	if (!bKeys) return;
}

//--------------------------------------------------------------
void ofxSurfingPresets::addKeysListeners()
{
	ofAddListener(ofEvents().keyPressed, this, &ofxSurfingPresets::keyPressed);
	ofAddListener(ofEvents().keyReleased, this, &ofxSurfingPresets::keyReleased);
}
//--------------------------------------------------------------
void ofxSurfingPresets::removeKeysListeners()
{
	ofRemoveListener(ofEvents().keyPressed, this, &ofxSurfingPresets::keyPressed);
	ofRemoveListener(ofEvents().keyReleased, this, &ofxSurfingPresets::keyReleased);
}

//--------------------------------------------------------------
void ofxSurfingPresets::exit()
{
	doSaveCurrent();
}

//--------------------------------------------------------------
void ofxSurfingPresets::setLogLevel(ofLogLevel level)
{
	ofSetLogLevel(__FUNCTION__, level);
}

//--------------------------------------------------------------
void ofxSurfingPresets::setActive(bool b)
{
	// disables all keys interaction listeners 

	//bMODE_Active = b;

	if (!b)
	{
		removeKeysListeners();
	}
	else
	{
		addKeysListeners();
	}
}

//--------------------------------------------------------------
void ofxSurfingPresets::setGuiVisible(bool b)
{
	bGui = b;
	bGui_Global = b;//TODO:
}

//// all params
////--------------------------------------------------------------
//void ofxSurfingPresets::Changed_Params(ofAbstractParameter &e)
//{
//	if (bDISABLE_CALLBACKS) return;
//
//	{
//		std::string name = e.getName();
//
//		// exclude debugs
//		if (name != "exclude"
//			&& name != "exclude")
//		{
//			ofLogNotice(__FUNCTION__) << "Changed_Params: " << name << " : " << e;
//
//		}
//
//		// params
//		if (name == "")
//		{
//		}
//	}
//}

//--------------------------------------------------------------
void ofxSurfingPresets::Changed_Control(ofAbstractParameter& e)
{
	if (bDISABLE_CALLBACKS) return;

	std::string name = e.getName();

	// Exclude debugs
	//if (name != "exclude" && name != "exclude")
	{
		ofLogNotice(__FUNCTION__) << name << " : " << e;
	}

	//--

	if (0) {}

	//--

	// Preset Index

	else if (name == index.getName())
	{
		index = ofClamp(index.get(), index.getMin(), index.getMax()); // clamp inside dir files amount limits

		// Changed?

		if (index.get() != index_PRE)
		{
			if (index_PRE != -1)
			{
				ofLogNotice(__FUNCTION__) << "\n\n  Changed \n  Preset Index : "
					<< ofToString(index_PRE) << " > " << ofToString(index)
					<< "      \t(" << ofToString(keyCommandsChars[index_PRE]) << " > " << ofToString(keyCommandsChars[index]) << ")"
					<< "\n";
			}

			//--

			// 1. Common Load but AutoSave

			if (!bKeyCtrl && !bKeyAlt) // Ctrl nor Alt not pressed
			{
				// Autosave

				if (bEditMode)
				{
					if (dir.size() > 0 && index_PRE < dir.size())
					{
						filePath = getFilepathForIndexPreset(index_PRE);
						save(filePath);
					}
					else { ofLogError(__FUNCTION__) << "Preset Index points an out of range file!"; }
				}

				index_PRE = index;

				//--

				// Load

				ofLogNotice(__FUNCTION__) << index.getName() + " : " << ofToString(index);

				if (dir.size() > 0 && index < dir.size())
				{
					filePath = getFilepathForIndexPreset(index);
					load(filePath);

					//-

#if defined(USE__OFX_SURFING_PRESET__MIDI__) || defined(USE__OFX_SURFING_CONTROL__OFX_REMOTE_PARAMETERS__SERVER)
//#ifdef INCLUDE__OFX_SURFING_PRESET__OFX_MIDI_PARAMS
					refreshToggleNotes();
#endif
				}
				else
				{
					ofLogError(__FUNCTION__) << "File out of range";
				}
			}

			//--

			// 2. Save / Copy

			// Save to clicked preset index
			// Ctrl pressed. Alt not pressed

			else if (bKeyCtrl && !bKeyAlt)
			{
				filePath = getFilepathForIndexPreset(index);
				save(filePath);

#if defined(USE__OFX_SURFING_PRESET__MIDI__) || defined(USE__OFX_SURFING_CONTROL__OFX_REMOTE_PARAMETERS__SERVER)
				//#ifdef INCLUDE__OFX_SURFING_PRESET__OFX_MIDI_PARAMS
				refreshToggleNotes();
#endif
				ofLogNotice(__FUNCTION__) << "PRESET COPY!";

				index_PRE = index;
			}

			//--

			//TODO:

			// 3. Swap

			// (from/to) pre/current index
			// Ctrl not pressed. Alt pressed

			else if (!bKeyCtrl && bKeyAlt)
			{
				// Rename target to source
				string _fFrom = getFilepathForIndexPreset(index_PRE);
				string _fTo = getFilepathForIndexPreset(index);
				ofFile f;

				bool bf = f.open(_fTo);
				bool bt = f.renameTo(_fFrom, true, true);

				// Save current to
				save(_fTo);

				if (bf && bt) {
					ofLogNotice(__FUNCTION__) << "PRESET SWAP!";
					ofLogNotice(__FUNCTION__) << _fFrom << " <-> " << _fTo;
				}
				else {
					ofLogError(__FUNCTION__) << "WRONG SWAP!";
				}

				index_PRE = index;
			}
		}

		//--

		// Index not changed, 
		// but we flag it happens to catch a retrig. sometimes useful	
		else
		{
			bIsRetrigged = true;
		}
	}

	//--

	else if (name == bSave.getName() && bSave)
	{
		bSave = false;
		save(filePath);
	}
	else if (name == bLoad.getName() && bLoad)
	{
		bLoad = false;
		load(filePath);
	}
	else if (name == bNewPreset.getName() && bNewPreset)
	{
		bNewPreset = false;
		doNewPreset();
	}
	else if (name == bRefresh.getName() && bRefresh)
	{
		bRefresh = false;
		doRefreshFiles();
	}
	else if (name == bSetPathPresets.getName() && bSetPathPresets)
	{
		setPath();
	}

	//--

//		//TODO:
//		// Workflow
//		//link minimize controls..
//		else if (name == bMinimize.getName())
//		{
//			//skip
//
//			bDISABLE_CALLBACKS = true;
//
//			//ui.bMinimize = bMinimize;//linked
//			bMinimize_Editor = bMinimize;
//			bMinimize_Params = bMinimize;
//
//#ifdef USE__OFX_SURFING_PRESETS__OFX_SURFING_PLAYER 
//			playerSurfer.bMinimize_Player = bMinimize;
//#endif
//
//			bDISABLE_CALLBACKS = false;
//		}
}

#ifdef USE_TOGGLE_TRIGGERS
//--------------------------------------------------------------
void ofxSurfingPresets::Changed_Params_PresetToggles(ofAbstractParameter& e)
{
	if (bDISABLE_CALLBACKS) return;

	std::string name = e.getName();

	bool bdone = false;

	for (int i = 0; i <= index.getMax() && i < notesIndex.size(); i++)
	{
		// true
		if (notesIndex[i].get() && name == notesIndex[i].getName())
		{
			index = i;
			ofLogNotice(__FUNCTION__) << name << " : TRUE";
			bdone = true;
			continue;
		}
	}
	if (!bdone) return; // Not any detected note true

	//-

	// Make exclusive. All others to false
	for (int i = 0; i <= index.getMax() && i < notesIndex.size(); i++)
	{
		if (i != index && notesIndex[i].get())
		{
			notesIndex[i] = false;
		}
	}
}
#endif

#ifdef USE__OFX_SURFING_CONTROL__OFX_REMOTE_PARAMETERS__SERVER
//--------------------------------------------------------------
void ofxSurfingPresets::Changed_Params_Preset(ofAbstractParameter& e)
{
	//ofLogVerbose(__FUNCTION__);

	bSyncRemote = true;
	// update on next frame to reduce calls when multiple params changed.

	//// Note that if you use the GUI the client does not update automatically. If you want the client to update
	//// you will need to call paramServer.syncParameters() whenever a parameter does change.
	//remoteServer.syncParameters();
}
#endif

////--------------------------------------------------------------
//void ofxSurfingPresets::setKey_MODE_App(int k)
//{
//	key_MODE_App = k;
//}

//--------------------------------------------------------------
void ofxSurfingPresets::setPathGlobal(std::string s) // Must call before setup or addGroup. disabled by default
{
	ofLogNotice(__FUNCTION__) << s;
	path_Global = s;

	ofxSurfingHelpers::CheckFolder(path_Global);
}
//--------------------------------------------------------------
void ofxSurfingPresets::setPathPresets(std::string s)
{
	ofLogNotice(__FUNCTION__) << s;
	path_Presets = s;

	ofxSurfingHelpers::CheckFolder(path_Presets);
}

//--

//--------------------------------------------------------------
void ofxSurfingPresets::doRecallState()
{
	ofLogNotice(__FUNCTION__) << path_Global + path_filePreset + _ext;
	ofxSurfingHelpers::loadGroup(params_Preset, path_Global + path_filePreset + _ext);

	DONE_load = true;

	// Simple callback
	bIsDoneLoad = true;

	//bIsRetrigged = true;
}

//--------------------------------------------------------------
void ofxSurfingPresets::doStoreState()
{
	ofLogNotice(__FUNCTION__) << path_Global + path_filePreset + _ext;
	ofxSurfingHelpers::saveGroup(params_Preset, path_Global + path_filePreset + _ext);

	bResetDefined = true;

	DONE_save = true;

	// Simple callback
	bIsDoneSave = true;
}

//--------------------------------------------------------------
void ofxSurfingPresets::doSaveCurrent()
{
	ofLogNotice(__FUNCTION__) << filePath;
	save(filePath);
}

//--------------------------------------------------------------
void ofxSurfingPresets::load(int _index)
{
	ofLogNotice(__FUNCTION__) << _index;

	index = _index;
}

//--------------------------------------------------------------
void ofxSurfingPresets::load(std::string path)
{
	ofLogNotice(__FUNCTION__) << path;
	ofxSurfingHelpers::loadGroup(params_Preset, path);

	//-

	// Callback
	// MODE A. it's important if this line is before or after ofSerialize
	ofLogVerbose(__FUNCTION__) << "DONE LOAD";
	DONE_load = true;

	// Simple callback
	bIsDoneLoad = true;
}

//--------------------------------------------------------------
void ofxSurfingPresets::save(std::string path)
{
	ofLogNotice(__FUNCTION__) << path;
	ofxSurfingHelpers::saveGroup(params_Preset, path, false); // exclude debug

	//-

	// Callback
	// MODE A. it's important if this line is before or after ofSerialize
	ofLogVerbose(__FUNCTION__) << "DONE SAVE";
	DONE_save = true;

	// Simple callback
	bIsDoneSave = true;
}

//--------------------------------------------------------------
void ofxSurfingPresets::doNewPreset()
{
	//std::string ss = fileNames[index];

	int _i = dir.size();
	std::string s = ofToString(_i);
	if (_i < 10) s = "0" + s;
	std::string ss = name_Root + "_" + s;
	fileName = ss;
	filePath = path_Presets + "/" + ss + _ext;
	ofLogNotice(__FUNCTION__) << filePath;

	ofxSurfingHelpers::saveGroup(params_Preset, filePath);

	doRefreshFiles();

	for (int i = 0; i < fileNames.size(); i++)
	{
		if (fileNames[i] == fileName)
		{
			ofLogNotice(__FUNCTION__) << "file " << "[" << ofToString(i) << "] " << dir.getName(i);
			index = i;
			continue;
		}
	}

	//-

	//TODO:
	// Must fix!
	// Should be a method 
	// Autocreate notes for each index preset
#ifdef USE_TOGGLE_TRIGGERS
	int diff = index.getMax() - (notesIndex.size() - 1);
	if (diff <= 0) return;//trick?

	for (int i = 0; i < diff; i++) {
		std::string n = "Preset ";
		n += ofToString(notesIndex.size() + i);
		ofParameter<bool> b{ n, false };
		notesIndex.push_back(b);
		params_PresetToggles.add(b);

#ifdef INCLUDE__OFX_SURFING_PRESET__OFX_MIDI_PARAMS
		surfingMIDI.add(b);
#endif

		//doRecreateMidi();
	}
#endif
}

//--------------------------------------------------------------
void ofxSurfingPresets::doDeletePreset(int pos)
{
	int indexPre = index;

	// Last
	if (pos == -1)
	{
		index = index.getMax(); // Go to last. name will be updated
		ofFile::removeFile(filePath); // Remove last
		ofLogNotice(__FUNCTION__) << "Remove last: " << filePath;

		doRefreshFiles();

		// Load last
		if (dir.size() > 0) index = index.getMax();
	}
	// Current
	else
	{
		int i = index;
		std::string si = ofToString(i);
		if (i < 10) si = "0" + si;
		std::string ss = name_Root + "_" + si;
		fileName = ss;
		filePath = path_Presets + "/" + ss + _ext;
		ofLogNotice(__FUNCTION__) << "Remove: " << filePath;
		ofFile::removeFile(filePath);

		//-

		// Recreate
		doRefreshFilesAndRename();
	}

	//TODO: 
	// Workflow
	// Re set index position 
	if (indexPre >= dir.size() - 1)
	{
		index = dir.size() - 1;
	}
	else
	{
		index = indexPre;
	}
	//if (dir.size() > 0) index = indexPre;
	// load first file in dir
	//if (dir.size() > 0) index = 0;
	//else index = -1;
}

//--------------------------------------------------------------
void ofxSurfingPresets::doPopulatePresets(int amount)
{
	ofLogNotice(__FUNCTION__);

	doClearPresets(true);

	int _max;
	if (amount == -1)
	{
		_max = amountKitOfPresetsSize;
		//const int _max = dir.size();//same amount that files on folder
	}
	else _max = amount;

	for (int i = 0; i < _max - 1; i++)
	{
		index = i;
		doNewPreset();
		doSaveCurrent();
	}

	//// Workflow
	//amountButtonsPerRowClicker.setMax(_max);
	//amountButtonsPerRowClickerMini.setMax(_max);
	//amountButtonsPerRowClicker.set(_max / 3);
	//amountButtonsPerRowClickerMini.set(_max / 3);

	index = 0;
}

//--------------------------------------------------------------
void ofxSurfingPresets::doPopulatePresetsRandomized()
{
	ofLogNotice(__FUNCTION__);

	doClearPresets(true);

	const int _max = amountKitOfPresetsSize;
	//const int _max = dir.size();

	for (int i = 0; i < _max - 1; i++)
	{
		index = i;
		doNewPreset();
		doRandomizeParams(true);
		doSaveCurrent();
	}

	//// Workflow
	//amountButtonsPerRowClicker.setMax(_max);
	//amountButtonsPerRowClickerMini.setMax(_max);
	//amountButtonsPerRowClicker.set(_max / 3);
	//amountButtonsPerRowClickerMini.set(_max / 3);

	index = 0;
}

//--------------------------------------------------------------
void ofxSurfingPresets::doClearPresets(bool createOne)
{
	ofLogNotice(__FUNCTION__);

	// Remove all files
	for (int i = 0; i < dir.size(); i++)
	{
		ofFile file = dir[i];
		file.remove();
	}
	doRefreshFiles();

	if (createOne) doNewPreset();

	index = 0;
}

////--------------------------------------------------------------
//void ofxSurfingPresets::doCopyPreset()
//{
//	//TODO:
//
//	//std::string ss = fileNames[index];
//	//ss += "_";
//	//ss += ofToString(dir.size());
//	////ss += ofToString(index);
//
//	//fileName = ss;
//	//filePath = path_Presets + "/" + fileName + _ext;
//	//ofLogNotice(__FUNCTION__) << filePath;
//
//	//ofxSurfingHelpers::saveGroup(params_Preset, filePath);
//
//	//doRefreshFiles();
//
//	////TODO: fix name extension
//	////TODO: set index to new one
//	//
//	//for (int i = 0; i < fileNames.size(); i++)
//	//{
//	//	auto _ss = ofSplitString(fileNames[i], ".");
//	//	std::string _filename = "NoName";
//	//	if (_ss.size() > 0) _filename = _ss[0];
//
//	//	if (fileNames[i] == _filename)
//	//	{
//	//		ofLogNotice(__FUNCTION__) << "file " << "[" << ofToString(i) << "] " << dir.getName(i);
//	//		index = i;
//	//	}
//	//}
//}

//--------------------------------------------------------------
bool ofxSurfingPresets::doRefreshFiles()
{
	// Load dragged images folder
	ofLogNotice(__FUNCTION__) << "list files " << path_Presets;

	bool b = false;

	dir.listDir(path_Presets);
	dir.allowExt("JSON");
	dir.allowExt("json");
	dir.sort();

	// Log files on folder
	fileNames.clear();
	for (int i = 0; i < dir.size(); i++)
	{
		ofLogNotice(__FUNCTION__) << "file " << "[" << ofToString(i) << "] " << dir.getName(i);

		std::string _name = "NoName"; // without ext
		auto _names = ofSplitString(dir.getName(i), ".");
		if (_names.size() > 0) {
			_name = _names[0];
		}
		fileNames.push_back(_name);
	}

	index.setMin(0);
	if (dir.size() == 0) index.setMax(0);
	else
	{
		index.setMax(dir.size() - 1);

		// Workflow
		amountButtonsPerRowClicker.setMax(dir.size());
		amountButtonsPerRowClickerMini.setMax(dir.size());
	}

	index = index;

	//-

	b = (dir.size() > 0);
	//true if there's some file

	return b;

	//-

#ifdef USE_TOGGLE_TRIGGERS

	//TODO:
	ofRemoveListener(params_PresetToggles.parameterChangedE(), this, &ofxSurfingPresets::Changed_Params_PresetToggles);
	notesIndex.clear();
	params_PresetToggles.clear();

	for (int i = 0; i <= index.getMax(); i++)
	{
		std::string n = "Preset ";
		//n += ofToString(i < 10 ? "0" : "");
		n += ofToString(i);

		ofParameter<bool> b{ n, false };
		notesIndex.push_back(b);
		params_PresetToggles.add(b);
	}
	ofAddListener(params_PresetToggles.parameterChangedE(), this, &ofxSurfingPresets::Changed_Params_PresetToggles);

#ifdef USE__OFX_SURFING_PRESET__MIDI__
	doRecreateMidi();
#endif

	//#ifdef INCLUDE__OFX_SURFING_PRESET__OFX_MIDI_PARAMS
	//	// Splitted
	//	surfingMIDI.clear();
	//	surfingMIDI.add(params_Preset); // -> The ofParams of each Preset 
	//	surfingMIDI.add(params_PresetToggles); // -> To select the index preset by note/toggle (exclusive)
	//	surfingMIDI.add(index); // -> Add an int to select the index preset
	//#endif

#endif
}

//--------------------------------------------------------------
void ofxSurfingPresets::doRefreshFilesAndRename()
{
	ofLogNotice(__FUNCTION__) << "list files " << path_Presets;

	// Dir
	dir.listDir(path_Presets);
	dir.allowExt("JSON");
	dir.allowExt("json");
	dir.sort();

	// Rename all the files on the folder
	fileNames.clear();
	for (int i = 0; i < dir.size(); i++)
	{
		ofLogNotice(__FUNCTION__) << "file " << "[" << ofToString(i) << "] " << dir.getName(i);

		std::string si = ofToString(i);
		if (i < 10) si = "0" + si;
		std::string ss = name_Root + "_" + si;
		fileName = ss;
		filePath = path_Presets + "/" + ss + _ext;
		ofLogNotice(__FUNCTION__) << "Remove: " << filePath;

		ofFile f;
		f.open(dir[i].getAbsolutePath());
		f.renameTo(filePath);
		//f.renameTo(filePath, true, true);

		fileNames.push_back(fileName);
	}

	index.setMin(0);
	if (dir.size() == 0) index.setMax(0);
	else
	{
		index.setMax(dir.size() - 1);

		//// Workflow
		//amountButtonsPerRowClicker.setMax(dir.size());
		//amountButtonsPerRowClickerMini.setMax(dir.size());
	}
}

//--------------------------------------------------------------
void ofxSurfingPresets::setPath()
{
	ofLogNotice(__FUNCTION__) << "Set presets path";

	// Open the Open File Dialog
	std::string str = "Select the presets kit path.\n";
	ofFileDialogResult openFileResult = ofSystemLoadDialog(str, true);

	// Check if the user opened a file
	if (openFileResult.bSuccess)
	{
		path_Presets = openFileResult.getPath();
		path_Presets += "\\"; // windows
		ofLogNotice(__FUNCTION__) << "User selected a path: " << path_Presets;

		doRefreshFiles();
	}
	else
	{
		ofLogNotice(__FUNCTION__) << "User hit cancel";
	}
}

//--------------------------------------------------------------
void ofxSurfingPresets::doLoadPrevious()
{
	if (dir.size() == 0) return;

	ofLogNotice(__FUNCTION__);
	if (bCycled)
	{
		int i = index.get();
		if (i == index.getMin())
		{
			index.set(index.getMax());
		}
		else index--;
	}
	else index--;
}

//--------------------------------------------------------------
void ofxSurfingPresets::doLoadNext()
{
	if (dir.size() == 0) return;

	ofLogNotice(__FUNCTION__);
	if (bCycled)
	{
		int i = index.get();
		if (i == index.getMax())
		{
			index.set(index.getMin());
		}
		else index++;
	}
	else index++;
}

//--------------------------------------------------------------
void ofxSurfingPresets::doRandomizeIndex() {
	int icur = index.get();
	int i = ofRandom(index.getMin(), index.getMax() + 1);
	if (i == icur) {
		i++;
		i = i % index.getMax();
	}
	ofLogNotice(__FUNCTION__) << i;
	load(i);
}

////TODO:
//// should think a better use..
////--------------------------------------------------------------
//void ofxSurfingPresets::doSortParams(int i) {
//	//TODO: this is a workaround to autopopulate presets on ofxSurfingPresets...
//
//	ofLogNotice(__FUNCTION__);
//
//	for (int i = 0; i < params_Preset.size(); i++)
//	{
//		//// apply only if enabled
//		//auto &pe = params_Preset[i];
//		//auto type = pe.type();
//		//bool isBool = type == typeid(ofParameter<bool>).name();
//		//if (isBool) {
//		//	ofParameter<bool> pb = pe.cast<bool>();
//		//	if (!pb.get()) continue;
//		//}
//
//		//-
//
//		auto &p = params_Preset[i];
//
//		//float v;
//		//if (p.type() == typeid(ofParameter<float>).name())
//		//{
//		//	ofParameter<float> pr = p.cast<float>();
//		//	pr = i;
//		//}
//		//else 
//		if (p.type() == typeid(ofParameter<int>).name())
//		{
//			ofParameter<int> pr = p.cast<int>();
//			pr = i;
//		}
//	}
//
//	bIsRetrigged = true;
//}

//--------------------------------------------------------------
void ofxSurfingPresets::doRandomizeParams(bool bSilent) {
	ofLogNotice(__FUNCTION__);

	//TODO:
	// this is not recursive inside the group content!
	// get from ImHelpers. AddGroup iterate groups

	for (int i = 0; i < params_Preset.size(); i++)
	{
		auto& p = params_Preset[i];

		if (p.type() == typeid(ofParameter<float>).name())
		{
			ofParameter<float> pr = p.cast<float>();
			float v = ofRandom(pr.getMin(), pr.getMax());
			if (bSilent) pr.setWithoutEventNotifications(v);
			else pr.set(v);
			ofLogNotice(__FUNCTION__) << pr.getName() << " = " << pr.get();
		}

		else if (p.type() == typeid(ofParameter<int>).name())
		{
			ofParameter<int> pr = p.cast<int>();
			int v = ofRandom(pr.getMin(), pr.getMax());
			if (bSilent) pr.setWithoutEventNotifications(v);
			else pr.set(v);
			ofLogNotice(__FUNCTION__) << pr.getName() << " = " << pr.get();
		}

		// include booleans
		else if (p.type() == typeid(ofParameter<bool>).name())
		{
			ofParameter<bool> pr = p.cast<bool>();
			bool b = (ofRandom(1.f) >= 0.5f);
			if (bSilent) pr.setWithoutEventNotifications(b);
			else pr.set(b);
			ofLogNotice(__FUNCTION__) << pr.getName() << " = " << pr.get();
		}
	}

	//--

	// Undo Engine
#ifdef USE__OFX_SURFING__OFX_SURFING_UNDO_HELPER 
	undoManager.doSaveUndoWhenAuto();
#endif

	//--

	if (!bSilent) bIsRetrigged = true;
}

//--------------------------------------------------------------
void ofxSurfingPresets::doResetParams(bool bSilent) {
	ofLogNotice(__FUNCTION__);

	for (int i = 0; i < params_Preset.size(); i++)
	{
		auto& p = params_Preset[i];

		if (p.type() == typeid(ofParameter<float>).name())
		{
			ofParameter<float> pr = p.cast<float>();
			if (bSilent) pr.setWithoutEventNotifications(pr.getMin());
			else pr.set(pr.getMin());
			ofLogNotice(__FUNCTION__) << pr.getName() << " = " << pr.get();

		}

		else if (p.type() == typeid(ofParameter<int>).name())
		{
			ofParameter<int> pr = p.cast<int>();
			if (bSilent) pr.setWithoutEventNotifications(pr.getMin());
			else pr.set(pr.getMin());
			ofLogNotice(__FUNCTION__) << pr.getName() << " = " << pr.get();
		}

		// include booleans
		else if (p.type() == typeid(ofParameter<bool>).name())
		{
			ofParameter<bool> pr = p.cast<bool>();
			bool b = false;
			if (bSilent) pr.setWithoutEventNotifications(b);
			else pr.set(b);
			ofLogNotice(__FUNCTION__) << pr.getName() << " = " << pr.get();
		}
	}

	if (!bSilent) bIsRetrigged = true;
}
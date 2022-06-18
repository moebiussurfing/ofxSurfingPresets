#include "ofxSurfingPresets.h"


//--------------------------------------------------------------
ofxSurfingPresets::ofxSurfingPresets()
{
	// -> TODO: BUG? 
	// it seems than requires to be false when using multi-context/instances
	// if is setted to true, sometimes it hangs and gui do not refresh/freezes.
	bAutoDraw = false;

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

	setKeyFirstChar('1');
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
	bGui_ClickerFloating.set("CLICKER", true);
	bGui_Parameters.set("PARAMETERS", false);

	index.set("Index", 0, 0, 0);

	bCycled.set("Cycled", true);
	bSave.set("SAVE", false);
	bLoad.set("LOAD", false);
	bAutoSave.set("AUTOSAVE", true);
	bAutoSaveTimed.set("AutoSave Timed", false);
	bNewPreset.set("NEW", false);
	bSetPathPresets.set("PATH", false);
	bRefresh.set("REFRESH", false);

	bDebug.set("Debug", true);
	bKeys.set("Keys", true);
	bKeySpace.set("Key Space", true);
	bKeysArrows.set("Keys Arrows", true);
	//bHelp.set("HELP", false);

	//bAlignWindowsX.makeReferenceTo(guiManager.bAlign)

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
	//params_Control.add(bAlignWindowsX);
	//params_Control.add(bAlignWindowsY);
	params_Control.add(bMinimize);

	//params_Control.add(bGui_Global);
	params_Control.add(bGui);
	params_Control.add(bGui_ClickerFloating);
	params_Control.add(bGui_Editor);
	params_Control.add(bGui_Parameters);
#ifdef USE__OFX_SURFING_PRESETS__OFX_SURFING_PLAYER
	params_Control.add(playerSurfer.bGui);
#endif

	//params_Control.add(guiManager.bMinimize);
	//params_Control.add(bCycled);
	//params_Control.add(bGui_Parameters);
	//params_Control.add(bGui_ClickerSimple);
	//params_Control.add(bGui_ClickerFloating);
	//params_Control.add(bAutoSave);
	//params_Control.add(bAutoSaveTimed);

	ofAddListener(params_Control.parameterChangedE(), this, &ofxSurfingPresets::Changed_Control);

	//--

	// Params AppSettings

	params_AppSettings.clear();
	params_AppSettings.setName("ofxSurfingPresets_AppSettings");

	if (bEnableSettingsHandle)
	{
		params_AppSettings.add(bGui);
		params_AppSettings.add(bGui_ClickerFloating);
	}

	params_AppSettings.add(bGui_Editor);
	params_AppSettings.add(bGui_Parameters);
	params_AppSettings.add(bGui_ClickerSimple);
	params_AppSettings.add(index);
	params_AppSettings.add(bAutoSave);
	params_AppSettings.add(bAutoSaveTimed);
	params_AppSettings.add(bCycled);
	params_AppSettings.add(bKeys);

#ifdef USE__OFX_SURFING_PRESETS__OFX_SURFING_PLAYER 
	params_AppSettings.add(randomTypePlayIndex);
#endif

	params_AppSettings.add(bMinimize);
	//params_AppSettings.add(guiManager.bMinimize);
	//params_AppSettings.add(bMinimize_Params);
	//params_AppSettings.add(bMinimize_Editor);

	//params_AppSettings.add(guiManager.bExtra);
	//params_AppSettings.add(guiManager.bAutoResize);

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

	//bLinkWindows.makeReferenceTo(guiManager.bLinkWindows);

	bAutoResize.makeReferenceTo(guiManager.bAutoResize);

	//--

	// Player

#ifdef USE__OFX_SURFING_PRESETS__OFX_SURFING_PLAYER

	playerSurfer.setPathGlobal(path_Global);
	//playerSurfer.setNameGuiToggle(name_Root);

	//TODO:
	// Split change gui toggle too. add another label ?
	//playerSurfer.setNameSubPanel("Presets");

	// queue settings
	//params_AppSettings.add(playerSurfer.params_AppSettings);

	//--

	// Define the behaviors to trig when player trigs the callback!
	//--------------------------------------------------------------
	listener_Beat = playerSurfer.bPlayerBeatBang.newListener([this](bool& b) {
		ofLogNotice("BEAT: ") << (b ? "TRUE" : "FALSE");

		//if (playerSurfer.bPlay) // gui bangs are bypassed if not..
		{
			switch (randomTypePlayIndex)
			{
			case 0: doLoadNext(); break;
			case 1: doRandomizeIndex(); break;
			case 2: doRandomizeParams(); break;
			}
		}
		});

#endif

	//----

	params_FloatClicker.clear();
	params_FloatClicker.setName("FLOAT CLICKER");
	params_FloatClicker.add(amountButtonsPerRowClickerFloat);
	params_FloatClicker.add(bResponsiveButtons_ClickerFloating);
	params_FloatClicker.add(bExtra_ClickerFloating);
	params_FloatClicker.add(bAutoResize_ClickerFloating);
	params_FloatClicker.add(bMinimize_Editor);

	params_AppSettings.add(params_FloatClicker);

	//-

	params_ClickerSimple.clear();
	params_ClickerSimple.setName("CLICKER SIMPLE");
	params_ClickerSimple.add(amountButtonsPerRowClickerMini);
	params_ClickerSimple.add(bResponsiveButtons);

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

	setupGui();

	//--

	// Files
	doRefreshFiles();

	//--

	//startup(); // called from AddGroup
}

//--------------------------------------------------------------
void ofxSurfingPresets::setupGui()
{
	guiManager.setSettingsPathLabel("ofxSurfingPresets");

	//guiManager.setup(IM_GUI_MODE_INSTANTIATED);

	//--

	guiManager.setWindowsMode(IM_GUI_MODE_WINDOWS_SPECIAL_ORGANIZER);
	guiManager.setup();

	guiManager.addWindowSpecial(bGui_ClickerFloating);
	guiManager.addWindowSpecial(bGui_Parameters);
	guiManager.addWindowSpecial(bGui_Editor);

	guiManager.startup();

	//--

	//TODO:
	// linked to be exposed public. 
	// could be useful in some scenarios.
	guiManager.bMinimize.makeReferenceTo(bMinimize);
	//bMinimize.makeReferenceTo(guiManager.bMinimize);
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

	//Force default states

	bGui_ClickerFloating = true;
	bGui_Parameters = false;
	bGui_Editor = false;
	guiManager.getWindowsSpecialsGuiToggle().set(false);
	guiManager.getWindowsAlignHelpersGuiToggle().set(false);
	//guiManager.set

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

	refreshWindowsNames();

	//--

	// if no settings yet, set a default 3 presets per row.
	// NOTICE that usually use it for 3 rows of 3 presets matrices. with a total of 9 presets!
	if ((amountButtonsPerRowClickerFloat == -1) || (amountButtonsPerRowClickerMini == -1)) {
		setAmountButtonsPerRowClickers(3);
	}

	// Workflow

	// Force Load first preset
	//index = 0;
}

//--------------------------------------------------------------
void ofxSurfingPresets::update(ofEventArgs& args)
{
	// auto reload preset when clicked. that's to undo current editing!
	if (bAutoTrigMode)
		if (isRetrigged())
		{
			doReload();
		}

	//--

	//// Align Windows Engine

	//if (guiManager.bLinkWindows)
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

	// Autosave

	if (bAutoSaveTimed && ofGetElapsedTimeMillis() - timerLast_Autosave > timeToAutosave)
	{
		bDISABLE_CALLBACKS = true;

		ofxSurfingHelpers::saveGroup(params_AppSettings, path_Global + path_Params_Control);

		doSaveCurrent();

		bDISABLE_CALLBACKS = false;

		timerLast_Autosave = ofGetElapsedTimeMillis();
		ofLogNotice(__FUNCTION__) << "Autosaved DONE";
	}

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

	if (bGui)
	{
		//TODO: should be done manually to avoid some locking trouble when multiinstances
		draw_ImGui();

		//-

#ifdef INCLUDE__OFX_SURFING_PRESET__OFX_MIDI_PARAMS
		surfingMIDI.draw();
#endif

#ifdef INCLUDE__OFX_SURFING_PRESET__OFX_PARAMETER_MIDI_SYNC
		surfingMIDI.drawImGui();
#endif
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
	if (bGui_Editor)
	{
		// Widgets sizes
		float _w100;
		float _w50;
		float _w33;
		float _w25;
		float _h;

		//-

		// 1. Editor
		{
			std::string n;
			n = name_Window_Editor;

			ImGui::PushID(("##" + n + params_Preset.getName()).c_str());
			{
				if (guiManager.beginWindowSpecial(bGui_Editor))
				{
					//guiManager.AddLabelBig(params_Preset.getName(), false);

					_w100 = getWidgetsWidth(1);
					_w50 = getWidgetsWidth(2);
					_w33 = getWidgetsWidth(3);
					_w25 = getWidgetsWidth(4);
					_h = getWidgetsHeightUnit();

					//--

					guiManager.Add(bMinimize_Editor, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);

					if (!bMinimize_Editor) guiManager.Add(bKeys, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);

					if (!bMinimize_Editor)
					{
						//ImGui::Text(path_Global.data());
						//ImGui::Text(filePath.data());

						guiManager.AddSpacing();

						std::string ss;
						if (dir.size() == 0) ss = "NO PRESETS";
						else ss = ofToString(index) + "/" + ofToString(index.getMax());
						ImGui::Text(ss.data());

						if (bMinimize_Editor) ImGui::Text(fileName.data()); // -> using text input below
						//if (guiManager.bExtra) ImGui::Text(path_Presets.data()); // -> show path
					}

					//--

					// 1. Scrollable list

					if (!fileNames.empty())
					{
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
					guiManager.Add(index);

					// Browse
					if (bMinimize_Editor)
					{
						ImGui::PushButtonRepeat(true);
						{
							if (ImGui::Button("<", ImVec2(_w50, _h * 1.5)))
							{
								doLoadPrevious();
							}
							ImGui::SameLine();
							if (ImGui::Button(">", ImVec2(_w50, _h * 1.5)))
							{
								doLoadNext();
							}
						}
						ImGui::PopButtonRepeat();
					}

					//--

					// Simple Clicker 
					// (Inner. not floating) 
					if (!bMinimize_Editor)
						if (bGui_ClickerSimple)
						{
							draw_ImGui_ClickerSimple(false, true, false);
						}

					//--

					if (!bMinimize_Editor)
					{
						ImGui::PushButtonRepeat(true);
						{
							if (ImGui::Button("<", ImVec2(_w50, _h * 1.5)))
							{
								doLoadPrevious();
							}
							ImGui::SameLine();
							if (ImGui::Button(">", ImVec2(_w50, _h * 1.5)))
							{
								doLoadNext();
							}
						}
						ImGui::PopButtonRepeat();

						guiManager.Add(bSave, OFX_IM_BUTTON_SMALL, 2, true);
						guiManager.Add(bLoad, OFX_IM_BUTTON_SMALL, 2, false);

						guiManager.Add(bNewPreset, OFX_IM_BUTTON_SMALL, 2, true);
						guiManager.Add(bAutoSave, OFX_IM_TOGGLE_SMALL_BORDER_BLINK, 2, false);
					}

					//--

					if (!bMinimize_Editor)
					{
						// Clicker

						guiManager.Add(bGui_ClickerFloating, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);

						// Parameters

						guiManager.Add(bGui_Parameters, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);

						// Player

#ifdef USE__OFX_SURFING_PRESETS__OFX_SURFING_PLAYER 
						guiManager.Add(playerSurfer.bGui, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
						if (playerSurfer.bGui)
						{
							guiManager.Indent();
							guiManager.Add(playerSurfer.bPlay, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
							if (!bMinimize_Editor)
							{
								ofxImGuiSurfing::AddCombo(randomTypePlayIndex, randomTypesPlayNames);
							}
							guiManager.Unindent();
						}
#endif
					}

					//-

					// MIDI

#ifdef USE__OFX_SURFING_PRESET__MIDI__
					guiManager.Add(surfingMIDI.bGui, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
#endif
					//-

					if (!bMinimize_Editor)
					{
						bool bOpen = false;
						ImGuiTreeNodeFlags _flagt = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
						_flagt |= ImGuiTreeNodeFlags_Framed;

						if (ImGui::TreeNodeEx("TOOLS", _flagt))
						{
							// 1. Preset

							if (ImGui::TreeNodeEx("PRESET", _flagt))
							{
								guiManager.refreshLayout();
								_w100 = getWidgetsWidth(1);
								_w50 = getWidgetsWidth(2);
								_w33 = getWidgetsWidth(3);
								_w25 = getWidgetsWidth(4);
								_h = getWidgetsHeightUnit();

								if (ImGui::Button("NEW", ImVec2(_w50, _h)))
								{
									doNewPreset();
								}
								ImGui::SameLine();

								if (ImGui::Button("DELETE", ImVec2(_w50, _h))) ImGui::OpenPopup("DELETE?");

								if (ImGui::BeginPopupModal("DELETE?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
								{
									ImGui::Text("Current Preset will be deleted.\nThis operation cannot be undone!\n\n");
									ImGui::Separator();

									static bool dont_ask_me_next_time = false;
									ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
									ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
									ImGui::PopStyleVar();

									if (!dont_ask_me_next_time) {
										if (ImGui::Button("OK", ImVec2(120, 0))) {
											ofLogNotice(__FUNCTION__) << "DELETE";
											doDeletePreset(index);
											ImGui::CloseCurrentPopup();
										}
										ImGui::SetItemDefaultFocus();
										ImGui::SameLine();
										if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
									}
									else {
										ofLogNotice(__FUNCTION__) << "DELETE";
										doDeletePreset(index);
										ImGui::CloseCurrentPopup();
									}

									ImGui::EndPopup();
								}

								draw_ImGui_ToolsWidgets();

								ImGui::TreePop();
							}

							//--

							// 2. Kit

							if (ImGui::TreeNodeEx("KIT", _flagt))
							{
								guiManager.refreshLayout();
								_w100 = getWidgetsWidth(1);
								_w50 = getWidgetsWidth(2);
								_w33 = getWidgetsWidth(3);
								_w25 = getWidgetsWidth(4);
								_h = getWidgetsHeightUnit();

								guiManager.Add(bRefresh, OFX_IM_BUTTON_SMALL, 2, true);
								guiManager.Add(bSetPathPresets, OFX_IM_BUTTON_SMALL, 2, false);

								//TODO:
								//if (ImGui::Button("COPY", ImVec2(_w2, _h)))
								//{
								//	doCopyPreset();
								//}

								//TODO: show only on last preset
								//if (index == index.getMax())
								{
									if (ImGui::Button("CLEAR KIT", ImVec2(_w100, _h)))
									{
										ImGui::OpenPopup("CLEAR KIT?");
									}

									if (ImGui::BeginPopupModal("CLEAR KIT?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
									{
										ImGui::Text("User Kit will be erased.\nThis operation cannot be undone!\n\n");
										ImGui::Separator();

										static bool dont_ask_me_next_time = false;
										ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
										ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
										ImGui::PopStyleVar();

										if (!dont_ask_me_next_time) {
											if (ImGui::Button("OK", ImVec2(120, 0))) {
												ofLogNotice(__FUNCTION__) << "CLEAR";
												doClearPresets();
												ImGui::CloseCurrentPopup();
											}
											ImGui::SetItemDefaultFocus();
											ImGui::SameLine();
											if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
										}
										else {
											ofLogNotice(__FUNCTION__) << "CLEAR";
											doClearPresets();
											ImGui::CloseCurrentPopup();
										}

										ImGui::EndPopup();
									}

									if (ImGui::Button("RECREATE", ImVec2(_w100, _h)))
									{
										doRefreshFilesAndRename();
									}

									if (ImGui::Button("POPULATE", ImVec2(_w100, _h)))
									{
										doPopulatePresets();
									}

									if (ImGui::Button("POPU RND", ImVec2(_w100, _h)))
									{
										doPopulatePresetsRandomized();
									}
								}

								ImGui::TreePop();
							}

							ImGui::TreePop();
						}
					}

					//-

					// Extra

					static ofParameter<bool> bFiles{ "Files", false };

					if (!bMinimize_Editor)
					{
						ImGui::Spacing();
						ImGui::Spacing();

						guiManager.Add(guiManager.bExtra, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);

						if (guiManager.bExtra)
						{
							guiManager.Indent();
							{
								//--

								// Organizer aligners
								guiManager.Add(guiManager.getWindowsSpecialsGuiToggle(), OFX_IM_TOGGLE_BUTTON_ROUNDED);
								//guiManager.Add(guiManager.getWindowsAlignHelpersGuiToggle(), OFX_IM_TOGGLE_BUTTON_ROUNDED);
								guiManager.AddSpacing();

								//--

								// Keys
								//ofxImGuiSurfing::AddToggleRoundedButton(bKeys);

								// Smooth

								// AppExtra
								if (params_AppExtra.getName() != "-1")
								{
									static ofParameter<bool> b = params_AppExtra.getBool("Smooth");
									static ofParameter<float> v = params_AppExtra.getFloat("Speed");

									ofxImGuiSurfing::AddToggleRoundedButton(b);
									if (b)
									{
										guiManager.Indent();
										ofxImGuiSurfing::AddParameter(v, "%.1f");
										//guiManager.Add(v, OFX_IM_HSLIDER_SMALL_NO_LABELS, 2);
										guiManager.Unindent();
									}
								}

								ofxImGuiSurfing::AddToggleRoundedButton(bGui_ClickerSimple);
								if (bGui_ClickerSimple)
								{
									guiManager.Indent();

									ofxImGuiSurfing::AddToggleRoundedButton(bResponsiveButtons);
									if (bResponsiveButtons)
									{
										ofxImGuiSurfing::AddParameter(amountButtonsPerRowClickerMini);
										//ofxImGuiSurfing::AddStepperInt(amountButtonsPerRowClickerMini);
										//guiManager.Add(amountButtonsPerRowClickerMini, OFX_IM_SLIDER);
										//guiManager.Add(amountButtonsPerRowClickerMini, OFX_IM_HSLIDER_SMALL);
										//guiManager.Add(amountButtonsPerRowClickerMini, OFX_IM_DEFAULT);
										//guiManager.Add(amountButtonsPerRowClickerMini, OFX_IM_STEPPER);
									}

									guiManager.Unindent();
								}
								ofxImGuiSurfing::AddToggleRoundedButton(bCycled);
								ofxImGuiSurfing::AddToggleRoundedButton(bAutoSaveTimed);
								//ofxImGuiSurfing::AddToggleRoundedButton(bAutoSave);

								//-

								// Files
								ofxImGuiSurfing::AddToggleRoundedButton(bFiles);
								if (bFiles)
								{
									guiManager.Indent();
									{
										// Paths
										{
											bool bOpen = false;
											ImGuiTreeNodeFlags _flagt = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
											//_flagt |= ImGuiTreeNodeFlags_Framed;
											if (ImGui::TreeNodeEx("Paths", _flagt))
											{
												ImGui::Text(path_Presets.data()); // -> show path
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
									guiManager.Unindent();
								}

								//ofxImGuiSurfing::AddToggleRoundedButton(guiManager.bAutoResize);
							}
							guiManager.Unindent();

						}
					}

					//-

					// Extra Panel

					if (guiManager.bExtra)
					{
						if (!bMinimize_Editor)
						{
							guiManager.drawAdvanced();
						}
					}

					//-

					guiManager.endWindowSpecial();
				}
			}
			ImGui::PopID();
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingPresets::draw_ImGui_ClickerFloating()
{
	if (bGui_ClickerFloating)
	{
		// Widgets Sizes
		float _w1;
		float _w2;
		float _w3;
		float _w4;
		float _h;

		// 3. Floating Clicker

		std::string n;
		n = name_Window_ClickerFloating;

		ImGui::PushID(("##" + n + params_Preset.getName()).c_str());
		{
			if (guiManager.beginWindowSpecial(bGui_ClickerFloating))
			{
				guiManager.AddLabelBig(params_Preset.getName(), false);

				//--

				_w1 = getWidgetsWidth(1);
				_w2 = getWidgetsWidth(2);
				_w3 = getWidgetsWidth(3);
				_h = getWidgetsHeightUnit();

				//-

				// Minimize
				guiManager.Add(bMinimize, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);

				//// Align Windows
				//if (!bMinimize) guiManager.Add(guiManager.bLinkWindows, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);

				// Keys
				if (!bMinimize) guiManager.Add(bKeys, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);

				//----

				// Clicker Matrix
				{
					float sizey = ofxImGuiSurfing::getWidgetsHeightRelative() * 2;

					ofxImGuiSurfing::AddMatrixClickerLabels(index, keyCommandsChars, bResponsiveButtons_ClickerFloating, amountButtonsPerRowClickerFloat, true, sizey);

					//TODO:
					//using ptr
					//ofxImGuiSurfing::AddMatrixClickerLabels(index, (char *) keyCommandsChars, bResponsiveButtons_ClickerFloating, amountButtonsPerRowClickerFloat, true, sizey);
				}


				// Play
#ifdef USE__OFX_SURFING_PRESETS__OFX_SURFING_PLAYER 
				if (bMinimize)
				{
					guiManager.Add(playerSurfer.bPlay, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
				}
#endif

				//guiManager.AddSpacing();

				// Toggles to show Panels
				if (!bMinimize)
				{
					// Editor
					guiManager.Add(bGui_Editor, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);

					// Parameters
					guiManager.Add(bGui_Parameters, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);

					// Player 
#ifdef USE__OFX_SURFING_PRESETS__OFX_SURFING_PLAYER 
					guiManager.Add(playerSurfer.bGui, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
					//if (playerSurfer.bGui)
					{
						guiManager.Indent();
						guiManager.Add(playerSurfer.bPlay, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
						guiManager.Unindent();
					}
#endif
					//--

					// MIDI

#ifdef USE__OFX_SURFING_PRESET__MIDI__
					guiManager.Add(surfingMIDI.bGui, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
#endif
					//--

					// Extra
					// for Floating
					guiManager.Add(bExtra_ClickerFloating, OFX_IM_TOGGLE_BUTTON_ROUNDED);

					// Keys
					//guiManager.Add(bKeys, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);

					//--
				}

				// Extra

				if (!bMinimize)
					if (bExtra_ClickerFloating)
					{
						guiManager.Indent();
						{
							_w1 = getWidgetsWidth(1);
							_w2 = getWidgetsWidth(2);

							//--

							// Organizer aligners
							guiManager.Add(guiManager.getWindowsSpecialsGuiToggle(), OFX_IM_TOGGLE_BUTTON_ROUNDED);
							//guiManager.Add(guiManager.getWindowsAlignHelpersGuiToggle(), OFX_IM_TOGGLE_BUTTON_ROUNDED);
							guiManager.AddSpacing();

							//--

							{
								static bool bOpen = false;
								ImGuiColorEditFlags _flagw = (bOpen ? ImGuiWindowFlags_NoCollapse : ImGuiWindowFlags_None);
								if (ImGui::CollapsingHeader("LAYOUT", _flagw))
								{
									guiManager.refreshLayout();
									if (bResponsiveButtons_ClickerFloating)
									{
										guiManager.Add(amountButtonsPerRowClickerFloat, OFX_IM_SLIDER);
										//guiManager.Add(amountButtonsPerRowClickerFloat, OFX_IM_STEPPER);
									}

									guiManager.Add(bResponsiveButtons_ClickerFloating, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
									guiManager.Add(bAutoResize_ClickerFloating, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
								}
							}
						}
						guiManager.Unindent();
					}

				guiManager.endWindowSpecial();
			}
		}
		ImGui::PopID();
	}
}

//-----------------------------------------i---------------------
void ofxSurfingPresets::draw_ImGui_ClickerSimple()
{
	if (!bGui_ClickerSimple) bGui_ClickerSimple = true;
	draw_ImGui_ClickerSimple(false, false, false, true);
}

//-----------------------------------------i---------------------
void ofxSurfingPresets::draw_ImGui_ClickerSimple(bool bHeader, bool bMinimal, bool bShowMinimize, bool bNoExtras)
{
	if (!bGui_ClickerSimple) return;

	static bool bOpen = true;
	ImGuiTreeNodeFlags _flagt = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
	_flagt |= ImGuiTreeNodeFlags_Framed;

	bool b;

	if (bHeader) b = (ImGui::TreeNodeEx("PRESETS", _flagt));
	else b = true;

	if (b)
	{
		guiManager.refreshLayout();

		// Index
		if (!bNoExtras)guiManager.Add(index, OFX_IM_HSLIDER_SMALL_NO_LABELS);

		//guiManager.AddSpacing();

		// Clicker
		ofxImGuiSurfing::AddMatrixClickerLabels(index, keyCommandsChars, bResponsiveButtons, amountButtonsPerRowClickerMini, true, WIDGETS_HEIGHT / 2);
		//ofxImGuiSurfing::AddMatrixClicker(index, bResponsiveButtons, amountButtonsPerRowClickerMini, true, WIDGETS_HEIGHT / 2);

		if (!bNoExtras)
		{
			if (bShowMinimize) guiManager.Add(guiManager.bMinimize, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);

			if (!guiManager.bMinimize)
				if (!bMinimal)
				{
					//guiManager.Indent();

					// Editor window
					guiManager.Add(bGui_Editor, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);

					guiManager.refreshLayout();
					guiManager.Add(bSave, (bAutoSave ? OFX_IM_BUTTON_SMALL : OFX_IM_BUTTON_SMALL), 2, true);
					guiManager.Add(bAutoSave, OFX_IM_TOGGLE_SMALL, 2);

#ifdef USE__OFX_SURFING_PRESETS__OFX_SURFING_PLAYER 
					guiManager.Add(playerSurfer.bGui, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
					//if (playerSurfer.bGui) 
					{
						guiManager.Indent();
						guiManager.Add(playerSurfer.bPlay, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
						guiManager.Unindent();
				}
#endif
					//guiManager.Unindent();
		}
	}

		if (bHeader) ImGui::TreePop();
}
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
			guiManager.AddCombo(index, fileNames);
		}

		// minimize

		guiManager.Add(guiManager.bMinimize, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);

		// Index

		if (!guiManager.bMinimize)
		{
			ofxImGuiSurfing::AddParameter(index);
		}

		if (!guiManager.bMinimize)
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

		if (!guiManager.bMinimize)
		{
			//ofxImGuiSurfing::AddToggleRoundedButton(bGui);//main
			ofxImGuiSurfing::AddToggleRoundedButton(bGui_Editor);
			ofxImGuiSurfing::AddToggleRoundedButton(bGui_ClickerFloating);
		}

		if (bHeader) ImGui::TreePop();
	}
}

//--------------------------------------------------------------
void ofxSurfingPresets::draw_ImGui_Parameters()
{
	// 2. Preset Params

	if (bGui_Parameters)
	{
		if (params_Preset.getName() != "-1")
		{
			std::string n;
			n = name_Window_Parameters;

			ImGui::PushID(("##" + n + params_Preset.getName()).c_str());
			{
				if (guiManager.beginWindowSpecial(bGui_Parameters))
				{
					float _h = getWidgetsHeightUnit();
					float _w100 = getWidgetsWidth(1);
					float _w50 = getWidgetsWidth(2);

					//guiManager.AddLabelBig(params_Preset.getName(), false);

					//-

					// Minimize
					ofxImGuiSurfing::AddToggleRoundedButton(bMinimize_Params);

					guiManager.AddGroup(params_Preset);

					//-

					if (!bMinimize_Params)
					{
						ofxImGuiSurfing::AddSpacingSeparated();

						//--

						// Tools

						ImGuiTreeNodeFlags flagst;
						flagst = ImGuiTreeNodeFlags_None;
						flagst |= ImGuiTreeNodeFlags_Framed;
						//flagst |= ImGuiTreeNodeFlags_DefaultOpen;

						if (ImGui::TreeNodeEx("TOOLS", flagst))
						{
							draw_ImGui_ToolsWidgets();

							ImGui::TreePop();
						}

						//-

						//TODO: not implemented yet
						if (bReset.getName() != "-1")
						{
							ofxImGuiSurfing::AddSpacingSeparated();
							guiManager.Add(bReset, OFX_IM_BUTTON_SMALL);
						}

						//-

						//guiManager.Add(bGui_Editor, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
						//guiManager.Add(bGui_ClickerFloating, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
					}

					guiManager.endWindowSpecial();
				}
			}
			ImGui::PopID();
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingPresets::draw_ImGui_ToolsWidgets()
{
	float _w100 = ofxImGuiSurfing::getWidgetsWidth(1);
	float _w50 = ofxImGuiSurfing::getWidgetsWidth(2);
	float _h = getWidgetsHeightRelative();

	if (ImGui::Button("RECALL", ImVec2(_w50, _h)))
	{
		doRecallState();
	}
	ImGui::SameLine();
	if (ImGui::Button("STORE", ImVec2(_w50, _h)))
	{
		doStoreState();
	}

	if (ImGui::Button("RESET", ImVec2(_w50, _h)))
	{
		doResetParams();
	}
	ImGui::SameLine();
	if (ImGui::Button("RANDOM", ImVec2(_w50, _h)))
	{
		doRandomizeParams();
	}
}

//--------------------------------------------------------------
void ofxSurfingPresets::draw_ImGui()
{
	if (!bGui_Global) return;

	guiManager.begin();
	{
		draw_ImGui_ClickerFloating();

		//--

		draw_ImGui_Editor();

		//--

		draw_ImGui_Parameters();
	}
	guiManager.end();

	//--

	// Player

#ifdef USE__OFX_SURFING_PRESETS__OFX_SURFING_PLAYER 
	playerSurfer.draw();
#endif

}
//--------------------------------------------------------------
void ofxSurfingPresets::keyReleased(ofKeyEventArgs& eventArgs)
{
	if (!bKeys) return;

	const int& key = eventArgs.key;
	ofLogNotice(__FUNCTION__) << (char)key << " [" << key << "]";

	// modifiers
	bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
	bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
	bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
	bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);

	if (!mod_CONTROL) bKeyCtrl = false;
	if (!mod_ALT) bKeyAlt = false;
}

//--------------------------------------------------------------
void ofxSurfingPresets::keyPressed(ofKeyEventArgs& eventArgs)
{
	if (!bKeys) return;

	const int& key = eventArgs.key;
	ofLogNotice(__FUNCTION__) << (char)key << " [" << key << "]";

	// modifiers
	bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
	bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
	bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
	bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);

	if (mod_CONTROL) bKeyCtrl = true;
	if (mod_ALT) bKeyAlt = true;

	bool debug = 0;
	if (debug)
	{
		ofLogNotice(__FUNCTION__) << "mod_COMMAND: " << (mod_COMMAND ? "ON" : "OFF");
		ofLogNotice(__FUNCTION__) << "mod_CONTROL: " << (mod_CONTROL ? "ON" : "OFF");
		ofLogNotice(__FUNCTION__) << "mod_ALT: " << (mod_ALT ? "ON" : "OFF");
		ofLogNotice(__FUNCTION__) << "mod_SHIFT: " << (mod_SHIFT ? "ON" : "OFF");
	}

	if (0) {}

	else if (key == 'G') {
		setGuiVisibleToggle();
	}

	else if (key == OF_KEY_LEFT) {
		if (bKeysArrows) doLoadPrevious();
	}
	else if (key == OF_KEY_RIGHT) {
		if (bKeysArrows) doLoadNext();
	}
	if (key == ' ') {
		if (bKeySpace) doLoadNext();
	}
	else if (key == OF_KEY_BACKSPACE) {
		doRandomizeParams();
	}
	//else if (key == OF_KEY_RETURN) {
	//	doSaveCurrent();
	//}

	// Player
#ifdef USE__OFX_SURFING_PRESETS__OFX_SURFING_PLAYER 
	else if (key == OF_KEY_RETURN) { playerSurfer.setPlayToggle(); }
#endif

	// Preset Index Selector by key numbers
	else
		for (int i = 0; i < keyCommandsChars.size(); i++) {
			if (key == keyCommandsChars[i]) {
				load(i);
				//continue;
				return;
			}
		}
	//for (int i = 0; i < NUM_KEY_COMMANDS; i++) {
	//	if (key == keyCommandsChars[i]) {
	//		load(i);
	//		//continue;
	//		return;
	//	}
	//}
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

////--------------------------------------------------------------
//void ofxSurfingPresets::setName(string name)
//{
//
//}

//--------------------------------------------------------------
void ofxSurfingPresets::setActive(bool b)
{
	// disables all keys and mouse interaction listeners from the addon

	//bMODE_Active = b;

	if (!b)
	{
		removeKeysListeners();
		//removeMouseListeners();
	}
	else
	{
		addKeysListeners();
		//addMouseListeners();
	}
}

//--------------------------------------------------------------
void ofxSurfingPresets::setGuiVisible(bool b)
{
	bGui = b;
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

	{
		std::string name = e.getName();

		// Exclude debugs

		//if (name != "exclude" && name != "exclude")
		{
			ofLogNotice(__FUNCTION__) << name << " : " << e;
		}

		//--

		// Grouped callbacks to centralize and simplify

		//--

//		//TODO:
//		// Workflow
//
//		// Align Windows Engine
//
//		if (name == bAlignWindowsX.getName() && bAlignWindowsX.get())
//		{
//			bAlignWindowsX = false;
//			modeAlignWindows = SURFING_ALIGN_HORIZONTAL;//set as default mode
//			doAlignWindowsRefresh();
//		}
//
//		else if (name == bAlignWindowsY.getName() && bAlignWindowsY.get())
//		{
//			bAlignWindowsY = false;
//			modeAlignWindows = SURFING_ALIGN_VERTICAL;//set as default mode
//			doAlignWindowsRefresh();
//		}
//
//		else if (guiManager.bLinkWindows)
//		{
//			if ((name == bGui.getName()) ||
//				(name == bGui_ClickerFloating.getName()) ||
//				(name == bGui_Editor.getName()) ||
//				(name == bGui_Parameters.getName())
//#ifdef USE__OFX_SURFING_PRESETS__OFX_SURFING_PLAYER
//				||
//				(name == playerSurfer.bGui.getName())
//#endif
//				)
//			{
//				doAlignWindowsRefresh();
//			}
//		}

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

					if (bAutoSave)
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
					//dir.copyFromTo()

					// Rename target to source
					string _fFrom = getFilepathForIndexPreset(index_PRE);
					string _fTo = getFilepathForIndexPreset(index);
					ofFile f;

					bool bf = f.open(_fTo);
					bool bt = f.renameTo(_fFrom, true, true);

					// Save current to
					//save(_fFrom);
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
//			//guiManager.bMinimize = bMinimize;//linked
//			bMinimize_Editor = bMinimize;
//			bMinimize_Params = bMinimize;
//
//#ifdef USE__OFX_SURFING_PRESETS__OFX_SURFING_PLAYER 
//			playerSurfer.bMinimize_Player = bMinimize;
//#endif
//
//			bDISABLE_CALLBACKS = false;
//		}

		//--

		//else if (name == bMODE_Active.getName())
		//{
		//	setActive(bMODE_Active);
		//}

		//else if (name == bGui_Editor.getName())
		//{
		//	//TODO:
		//	//if (bGui_Editor)
		//	//{
		//	//	if (!bGui) bGui = true;
		//	//}
		//}

		//else if (name == bGui.getName())
		//{
		//	if (!bGui_Editor && !bGui_ClickerFloating)
		//	{
		//		bGui_Editor = true;
		//	}
		//}
	}
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
	ofLogVerbose(__FUNCTION__) << "DONE_load";
	DONE_load = true;

	//simple callback
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
	ofLogVerbose(__FUNCTION__) << "DONE_save";
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
	//amountButtonsPerRowClickerFloat.setMax(_max);
	//amountButtonsPerRowClickerMini.setMax(_max);
	//amountButtonsPerRowClickerFloat.set(_max / 3);
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
		doRandomizeParams(true);//silent
		doSaveCurrent();
	}

	//// Workflow
	//amountButtonsPerRowClickerFloat.setMax(_max);
	//amountButtonsPerRowClickerMini.setMax(_max);
	//amountButtonsPerRowClickerFloat.set(_max / 3);
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
		amountButtonsPerRowClickerFloat.setMax(dir.size());
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
		//amountButtonsPerRowClickerFloat.setMax(dir.size());
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
void ofxSurfingPresets::doRandomizeParams(bool bNoTrig) {
	ofLogNotice(__FUNCTION__);

	for (int i = 0; i < params_Preset.size(); i++)
	{
		//// apply only if enabled
		//auto &pe = params_Preset[i];
		//auto type = pe.type();
		//bool isBool = type == typeid(ofParameter<bool>).name();
		//if (isBool) {
		//	ofParameter<bool> pb = pe.cast<bool>();
		//	if (!pb.get()) continue;
		//}

		//-

		auto& p = params_Preset[i];

		float v;

		if (p.type() == typeid(ofParameter<float>).name())
		{
			ofParameter<float> pr = p.cast<float>();
			if (bNoTrig) pr.setWithoutEventNotifications(ofRandom(pr.getMin(), pr.getMax()));
			else pr = ofRandom(pr.getMin(), pr.getMax());
		}
		else if (p.type() == typeid(ofParameter<int>).name())
		{
			ofParameter<int> pr = p.cast<int>();
			if (bNoTrig) pr.setWithoutEventNotifications(ofRandom(pr.getMin(), pr.getMax() + 1));
			else pr = ofRandom(pr.getMin(), pr.getMax() + 1);
		}
	}

	if (!bNoTrig) bIsRetrigged = true;
}

//--------------------------------------------------------------
void ofxSurfingPresets::doResetParams() {
	ofLogNotice(__FUNCTION__);

	for (int i = 0; i < params_Preset.size(); i++)
	{
		auto& p = params_Preset[i];

		float v;

		if (p.type() == typeid(ofParameter<float>).name())
		{
			ofParameter<float> pr = p.cast<float>();
			pr = pr.getMin();
		}
		else if (p.type() == typeid(ofParameter<int>).name())
		{
			ofParameter<int> pr = p.cast<int>();
			pr = pr.getMin();
		}
	}

	bIsRetrigged = true;
}
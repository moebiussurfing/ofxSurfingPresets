#include "ofxSurfingPresets.h"


//--------------------------------------------------------------
ofxSurfingPresets::ofxSurfingPresets()
{
	// -> TODO: BUG?: 
	// it seems than requires to be false when using multi-context/instances
	// if is setted to true, sometimes it hangs and gui do not refresh/freezes.
	bAutoDraw = false;

	ofAddListener(ofEvents().update, this, &ofxSurfingPresets::update);
	//ofAddListener(ofEvents().draw, this, &ofxSurfingPresets::draw, OF_EVENT_ORDER_AFTER_APP);

	path_Global = "ofxSurfingPresets/"; // this is to folder all files to avoid mixing with other addons data
	path_Presets = "ofxSurfingPresets/Presets";
	path_Params_Control = "ofxSurfingPresets_Settings.xml";
	path_filePreset = "Preset";

	bDISABLECALLBACKS = true;

	setActive(true); // add key and mouse listeners

	//-

	// Customizable Key commands
	// '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
	// 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
	// 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
	// 'z', 'x', 'c', 'v', 'b', 'n', 'm'

	setKeyFirstChar('1');

	//setup();
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
	if (bDISABLECALLBACKS) return;

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
	ofSetLogLevel("ofxSurfingPresets", OF_LOG_NOTICE);
	//ofSetLogLevel("ofxSurfingPresets", OF_LOG_SILENT);

	//--

	// Params 

	bGui_Global.set("_Gui Global_", true);
	bGui_Global.setSerializable(false);
	bGui.set("SURFING PRESETS", true);
	bGui_Editor.set("EDITOR", true);
	bGui_InnerClicker.set("Inner Clicker", false);
	bGui_FloatingClicker.set("CLICKER", true);
	bGui_Parameters.set("PARAMETERS", false);

	bCycled.set("Cycled", true);
	bAutoSave.set("AutoSave", true);
	bAutoSaveTimer.set("AutoSave Timed", false);
	bNewPreset.set("New", false);
	bSave.set("Save", false);
	bLoad.set("Load", false);
	bSetPathPresets.set("PATH", false);
	bRefresh.set("REFRESH", false);

	index.set("Preset Index", 0, 0, 0);

	bDebug.set("Debug", true);
	bKeys.set("Keys", true);
	bKeySpace.set("Key Space", true);
	bKeysArrows.set("Keys Arrows", true);
	//bMODE_Active.set("Active", true);
	//bShowControl.set("Main Panel", true);
	//bHelp.set("HELP", false);	
	//MODE_App.set("APP MODE", 0, 0, NUM_MODES_APP - 1);
	//MODE_App_Name.set("", "");
	//MODE_App_Name.setSerializable(false);
	//bDebug.set("DEBUG", true);

	//-

	params_Control.clear();
	params_Control.setName("PRESETS CONTROL");
	params_Control.add(index);
	params_Control.add(bSave);
	params_Control.add(bNewPreset);
	params_Control.add(bLoad);
	params_Control.add(bSetPathPresets);
	params_Control.add(bRefresh);
	//params_Control.add(bGui_Parameters);
	//params_Control.add(bGui_InnerClicker);
	//params_Control.add(bShowControl);
	//params_Control.add(bGui_FloatingClicker);
	//params_Control.add(bAutoSave);
	//params_Control.add(bAutoSaveTimer);
	//params_Control.add(bCycled);

	ofAddListener(params_Control.parameterChangedE(), this, &ofxSurfingPresets::Changed_Control);

	//--

	// Params AppSettings

	params_AppSettings.clear();
	params_AppSettings.setName("ofxSurfingPresets_AppSettings");

	if (bEnableSettingsHandle)
	{
		params_AppSettings.add(bGui);
		params_AppSettings.add(bGui_FloatingClicker);
	}

	params_AppSettings.add(bGui_Editor);
	params_AppSettings.add(bGui_Parameters);
	params_AppSettings.add(bGui_InnerClicker);
	params_AppSettings.add(bAutoSave);
	params_AppSettings.add(bAutoSaveTimer);
	params_AppSettings.add(bCycled);
	params_AppSettings.add(bKeys);
	params_AppSettings.add(bMinimize_Params);
	params_AppSettings.add(index);
	//params_AppSettings.add(bMODE_Active);
	//params_AppSettings.add(bMinimize_Clicker);
	//params_AppSettings.add(bShowControl);
	//params_AppSettings.add(guiManager.bAutoResize);
	//params_AppSettings.add(guiManager.bExtra);
	//params_AppSettings.add(guiManager.bMinimize);

	//--

	// MIDI
#ifdef USE__OFX_SURFING_PRESET__MIDI__
	params_AppSettings.add(surfingMIDI.bGui);
#endif

	// Simple Smoother
#ifdef USE__OFX_SURFING_PRESETS__BASIC_SMOOTHER
	params_AppSettings.add(params_SmoothControl);
#endif

	//--

	// Player

#ifdef USE__OFX_SURFING_PRESETS__OFX_SURFING_PLAYER 
	surfingPlayer.setName(nameRoot);
	params_AppSettings.add(surfingPlayer.params_AppSettings);

	//TODO:
	// Split change gui toggle too. add another label ?
	surfingPlayer.setNameSubPanel("Presets");
	surfingPlayer.bGui = false;

	//-

	//--------------------------------------------------------------
	listener_Beat = surfingPlayer.bPlayerBeatBang.newListener([this](bool &b) {
		ofLogNotice("BEAT: ") << (b ? "TRUE" : "FALSE");

		//if (surfingPlayer.bPlay) // gui bangs are bypassed if not..
		{
			switch (randomTypePlayIndex)
			{
			case 0: doLoadNext(); break;
			case 1: doRandomizeIndex(); break;
			case 2: doRandomizeParams(); break;
			default:break;
			}
		}
	});
#endif

	//-

	params_FloatClicker.clear();
	params_FloatClicker.setName("FLOAT CLICKER");
	params_FloatClicker.add(amntBtnsFloatClicker);
	params_FloatClicker.add(respBtnsFloatClicker);
	params_FloatClicker.add(bExtraFloatClicker);
	params_FloatClicker.add(bAutoResizeFloatClicker);
	params_FloatClicker.add(bMinimize_Clicker);
	params_AppSettings.add(params_FloatClicker);
	//params_FloatClicker.add(bShowControl);

	//-

	params_InnerClicker.clear();
	params_InnerClicker.setName("INNER CLICKER");
	params_InnerClicker.add(amntBtns);
	params_InnerClicker.add(respBtns);
	params_AppSettings.add(params_InnerClicker);

	// External extra or smooth params
	if (params_AppExtra.getName() != "-1") params_AppSettings.add(params_AppExtra);

	// Exclude
	bSave.setSerializable(false);
	bNewPreset.setSerializable(false);
	bLoad.setSerializable(false);
	bSetPathPresets.setSerializable(false);
	bRefresh.setSerializable(false);
	//bMODE_Active.setSerializable(false);

	//-

	// All
	params.clear();
	params.setName("ALL PARAMS");
	params.add(params_Control);
	params.add(params_AppSettings);

	//--

	// Gui

	guiManager.setSettingsPathLabel("ofxSurfingPresets");

	guiManager.setup(IM_GUI_MODE_INSTANTIATED);

	// linked to be exposed public. could be useful in some situations.
	bMinimize.makeReferenceTo(guiManager.bMinimize);

	//--

	// Files
	doRefreshFiles();

	//--

	//startup(); // called from AddGroup

	//--
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

	bDISABLECALLBACKS = false;

	//-

	// Settings
	ofxSurfingHelpers::loadGroup(params_AppSettings, path_Global + path_Params_Control);

	//bMODE_Active = true;

	// Path for settings
	ofxSurfingHelpers::CheckFolder(path_Global);
	ofxSurfingHelpers::CheckFolder(path_Presets);

	//-

	// Files
	bool b = doRefreshFiles();

	// Initiate
	if (!b) doPopulatePresetsRandomized();

	//-

	//// Create first preset if folder it's empty
	//if (dir.size() == 0)
	//{
	//	doNewPreset();
	//}

	//ofxSurfingHelpers::loadGroup(params_Preset, path_Global + path_filePreset + _ext);

	// workflow
	// Load first preset
	//index = 0;

	//-

	doRefreshFiles();
}

//--------------------------------------------------------------
void ofxSurfingPresets::update(ofEventArgs & args)
{
	// Autosave
	//bAutoSave = false;

	if (bAutoSaveTimer && ofGetElapsedTimeMillis() - timerLast_Autosave > timeToAutosave)
	{
		bDISABLECALLBACKS = true;

		ofxSurfingHelpers::saveGroup(params_AppSettings, path_Global + path_Params_Control);

		bDISABLECALLBACKS = false;

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

		auto &g = params_Preset_Smoothed.castGroup();

		for (int i = 0; i < g.size(); i++)
		{
			auto &ap = g[i]; // ofAbstractParameter

			updateSmoothParam(ap);
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingPresets::updateSmoothParam(ofAbstractParameter& ap)
{
	auto type = ap.type();
	auto name = ap.getName();

	bool isFloat = type == typeid(ofParameter<float>).name();
	bool isInt = type == typeid(ofParameter<int>).name();
	bool isGroup = type == typeid(ofParameterGroup).name();

	// Float
	if (isFloat) {
		ofParameter<float> pVal = ap.cast<float>();
		ofParameter<float> pTar = params_Preset.getFloat(name); ;
		float v = pVal.get();
		ofxKuValueSmooth(v, pTar.get(), sp);
		//ofxSurfingHelpers::ofxKuValueSmooth(v, pTar.get(), sp);
		pVal = v;
	}
	// Int
	else if (isInt) {
		ofParameter<int> pVal = ap.cast<int>();
		ofParameter<int> pTar = params_Preset.getInt(name); ;
		int v = pVal.get();
		ofxKuValueSmooth(v, pTar.get(), sp);
		//ofxSurfingHelpers::ofxKuValueSmooth(v, pTar.get(), sp);
		pVal = v;
	}

	//TODO:
	// add more types

	// Group
	else if (isGroup) {
		auto &g = ap.castGroup();
		for (int i = 0; i < g.size(); i++) { // ofAbstractParameters
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
		// Panels sizes
		float xx = 10;
		float yy = 10;
		float ww = PANEL_WIDGETS_WIDTH;
		float hh = PANEL_WIDGETS_HEIGHT * 2;
		int pad = 10;

		// Widgets sizes
		float _w100;
		float _w50;
		float _w33;
		float _w25;
		float _h = WIDGETS_HEIGHT;

		std::string n;

		//-

		ImGuiWindowFlags flagsw = ImGuiWindowFlags_None;
		if (guiManager.bAutoResize) flagsw |= ImGuiWindowFlags_AlwaysAutoResize;

		ImGuiCond flagsCond = ImGuiCond_None;
		flagsCond |= ImGuiCond_FirstUseEver;

		//-

		// 1. Editor
		{
			ImGui::SetNextWindowSize(ImVec2(ww, hh), flagsCond);
			ImGui::SetNextWindowPos(ImVec2(xx, yy), flagsCond);

			if (nameRoot == "-1")
				n = "PRESETS EDITOR";
			else {
				n = nameRoot;
				n += " EDITOR";
			}

			ImGui::PushID(("##" + n + params_Preset.getName()).c_str());
			{
				IMGUI_SUGAR__WINDOWS_CONSTRAINTS;

				guiManager.beginWindow(n.c_str(), (bool*)&bGui_Editor.get(), flagsw);
				{
					//ImGui::Text(params_Preset.getName().c_str());

					guiManager.refreshLayout();
					_w100 = getWidgetsWidth(1);
					_w50 = getWidgetsWidth(2);
					_w33 = getWidgetsWidth(3);
					_w25 = getWidgetsWidth(4);
					_h = getWidgetsHeightUnit();

					//-

					guiManager.Add(guiManager.bMinimize, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
					if (!guiManager.bMinimize) guiManager.Add(bKeys, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);

					if (!guiManager.bMinimize)
					{
						//ImGui::Text(path_Global.data());
						//ImGui::Text(filePath.data());

						std::string ss;
						if (dir.size() == 0) ss = "NO PRESETS";
						else ss = ofToString(index) + "/" + ofToString(index.getMax());
						ImGui::Text(ss.data());

						if (guiManager.bMinimize) ImGui::Text(fileName.data()); // -> using text input below
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
					ImGui::PushItemWidth(_w50);
					guiManager.Add(index);
					//ofxImGuiSurfing::AddParameter(index);
					ImGui::PopItemWidth();
					//guiManager.Add(index, OFX_IM_SLIDER, 2);
					//ofxImGuiSurfing::AddParameter(index);

					// Browse
					if (guiManager.bMinimize)
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

					// Inner (not floating) Clicker Matrix

					if (bGui_InnerClicker)
					{
						draw_ImGui_MiniClicker();
					}

					//--

					if (!guiManager.bMinimize)
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

					//-

					// Clicker
					guiManager.Add(bGui_FloatingClicker, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);

					// Parameters
					guiManager.Add(bGui_Parameters, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);

					// Player
#ifdef USE__OFX_SURFING_PRESETS__OFX_SURFING_PLAYER 
					guiManager.Add(surfingPlayer.bGui, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
					if (surfingPlayer.bGui)
					{
						ImGui::Indent();
						guiManager.Add(surfingPlayer.bPlay, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
						if (!guiManager.bMinimize) {
							ImGui::PushItemWidth(_w100 * 0.7);
							ofxImGuiSurfing::AddCombo(randomTypePlayIndex, randomTypesPlayNames);
							ImGui::PushItemWidth(_w100 * 0.7);
						}
						ImGui::Unindent();
					}
#endif
					//-

					// MIDI
#ifdef USE__OFX_SURFING_PRESET__MIDI__
					guiManager.Add(surfingMIDI.bGui, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
#endif
					//-

					if (!guiManager.bMinimize)
					{
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

										if (ImGui::Button("POPULATE RND", ImVec2(_w100, _h)))
										{
											doPopulatePresetsRandomized();
										}
									}

									ImGui::TreePop();
								}

								ImGui::TreePop();
							}
						}
					}

					//-

					static ofParameter<bool> bFiles{ "Files", false };

					if (!guiManager.bMinimize)
					{
						ImGui::Spacing();

						guiManager.Add(guiManager.bExtra, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);

						if (guiManager.bExtra)
						{
							ImGui::Indent();
							{
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
										ImGui::Indent();
										ImGui::PushItemWidth(_w50);
										ofxImGuiSurfing::AddParameter(v, "%.1f");
										ImGui::PopItemWidth();
										//guiManager.Add(v, OFX_IM_HSLIDER_SMALL_NO_LABELS, 2);
										ImGui::Unindent();
									}
								}

								ofxImGuiSurfing::AddToggleRoundedButton(bGui_InnerClicker);
								if (bGui_InnerClicker)
								{
									ImGui::Indent();

									ofxImGuiSurfing::AddToggleRoundedButton(respBtns);
									if (respBtns)
									{
										//ImGui::PushItemWidth(WIDGET_PARAM_PADDING);
										//ofxImGuiSurfing::AddIntStepped(amntBtns);
										//ImGui::PopItemWidth();
										guiManager.Add(amntBtns, OFX_IM_STEPPER);
									}

									ImGui::Unindent();
								}
								ofxImGuiSurfing::AddToggleRoundedButton(bCycled);
								ofxImGuiSurfing::AddToggleRoundedButton(bAutoSaveTimer);
								//ofxImGuiSurfing::AddToggleRoundedButton(bAutoSave);

								//-

								// Files
								ofxImGuiSurfing::AddToggleRoundedButton(bFiles);
								if (bFiles)
								{
									ImGui::Indent();
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
												std::string ss = nameRoot + "_" + si;
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
									ImGui::Unindent();
								}

								ofxImGuiSurfing::AddToggleRoundedButton(guiManager.bAutoResize);
							}
							ImGui::Unindent();
						}
					}

					//-

					// Extra Panel

					if (guiManager.bExtra)
					{
						if (!guiManager.bMinimize)
						{
							guiManager.drawAdvanced();
						}
					}

					//-
				}
				guiManager.endWindow();
			}
			ImGui::PopID();
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingPresets::draw_ImGui_FloatingClicker()
{
	if (bGui_FloatingClicker)
	{
		// Panels Sizes
		float xx = 10;
		float yy = 10;
		float ww = PANEL_WIDGETS_WIDTH;
		float hh = PANEL_WIDGETS_HEIGHT * 2;
		int pad = 10;

		// Widgets Sizes
		float _w1;
		float _w2;
		float _w3;
		float _w4;
		float _h = WIDGETS_HEIGHT;

		// 3. Floating Clicker

		ImGuiWindowFlags flagsw = ImGuiWindowFlags_None;
		if (bAutoResizeFloatClicker) flagsw |= ImGuiWindowFlags_AlwaysAutoResize;

		std::string n;
		//n = "PRESETS";
		//n += " " + params_Preset.getName();
		//n = params_Preset.getName();
		//n = " PRESETS";
		n = bGui.getName();

		ImGui::PushID(("##" + n + params_Preset.getName()).c_str());
		{
			IMGUI_SUGAR__WINDOWS_CONSTRAINTS;

			guiManager.beginWindow(n.c_str(), (bool*)&bGui_FloatingClicker.get(), flagsw);
			{
				//ImGui::Text(params_Preset.getName().c_str());

				//ImGui::Checkbox("Key Ctrl", &bKeyCtrl);

				guiManager.refreshLayout();
				_w1 = getWidgetsWidth(1);
				_w2 = getWidgetsWidth(2);
				_w3 = getWidgetsWidth(3);
				_h = getWidgetsHeightUnit();

				//-

				// Minimize
				ofxImGuiSurfing::AddToggleRoundedButton(bMinimize_Clicker);

				// Keys
				if (!bMinimize_Clicker) guiManager.Add(bKeys, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);

				// Play
#ifdef USE__OFX_SURFING_PRESETS__OFX_SURFING_PLAYER 
				if (bMinimize_Clicker)
				{
					guiManager.Add(surfingPlayer.bPlay, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
				}
#endif

				//----

				// Clicker Matrix
				{
					float sizey = ofxImGuiSurfing::getWidgetsHeightRelative() * 2;

					ofxImGuiSurfing::AddMatrixClickerLabels(index, keyCommands, respBtnsFloatClicker, amntBtnsFloatClicker, true, sizey);
					//ofxImGuiSurfing::AddMatrixClickerLabels(index, (char *) keyCommands, respBtnsFloatClicker, amntBtnsFloatClicker, true, sizey);

					//ofxImGuiSurfing::AddMatrixClicker(index, respBtnsFloatClicker, amntBtnsFloatClicker, true, sizey);
				}

				// Toggles to show Panels
				if (!bMinimize_Clicker)
				{
					// Parameters
					guiManager.Add(bGui_Parameters, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);

					// Editor
					guiManager.Add(bGui_Editor, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);

					// Player 
#ifdef USE__OFX_SURFING_PRESETS__OFX_SURFING_PLAYER 
					guiManager.Add(surfingPlayer.bGui, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
					if (surfingPlayer.bGui)
					{
						ImGui::Indent();
						guiManager.Add(surfingPlayer.bPlay, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
						ImGui::Unindent();
					}
#endif
					// MIDI
#ifdef USE__OFX_SURFING_PRESET__MIDI__
					guiManager.Add(surfingMIDI.bGui, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
#endif
					//----

					// Extra
					// for Floating
					guiManager.Add(bExtraFloatClicker, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
					//ofxImGuiSurfing::AddToggleRoundedButton(bExtraFloatClicker);

					// Keys
					//guiManager.Add(bKeys, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
					}

				if (!bMinimize_Clicker)
					if (bExtraFloatClicker)
					{
						ImGui::Indent();
						{
							guiManager.refreshLayout();

							//----

							if (respBtnsFloatClicker)
							{
								guiManager.Add(amntBtnsFloatClicker, OFX_IM_SLIDER);
								//guiManager.Add(amntBtnsFloatClicker, OFX_IM_STEPPER);
							}
							ofxImGuiSurfing::AddToggleRoundedButton(respBtnsFloatClicker);
							ofxImGuiSurfing::AddToggleRoundedButton(bAutoResizeFloatClicker);
						}
						ImGui::Unindent();
					}

				//-
				}
			guiManager.endWindow();
			}
		ImGui::PopID();
		}
	}

//--------------------------------------------------------------
void ofxSurfingPresets::draw_ImGui_MiniClicker() {
	if (!bGui_InnerClicker) return;

	static bool bOpen = true;
	ImGuiTreeNodeFlags _flagt = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
	_flagt |= ImGuiTreeNodeFlags_Framed;

	if (ImGui::TreeNodeEx("PRESETS", _flagt))
	{
		guiManager.refreshLayout();

		// Index
		guiManager.Add(index, OFX_IM_HSLIDER_SMALL_NO_LABELS);
		//ofxImGuiSurfing::AddParameter(index);

		ImGui::Spacing();

		// Clicker
		//ofxImGuiSurfing::AddMatrixClicker(index, respBtns, amntBtns, true, WIDGETS_HEIGHT / 2);
		ofxImGuiSurfing::AddMatrixClickerLabels(index, keyCommands, respBtns, amntBtns, true, WIDGETS_HEIGHT / 2);

		// Editor window
		guiManager.Add(bGui_Editor, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
		//ofxImGuiSurfing::AddToggleRoundedButton(bGui_Editor);

		ImGui::TreePop();
	}
}

//--------------------------------------------------------------
void ofxSurfingPresets::draw_ImGui_Minimal()
{
	bool bOpen = false;
	ImGuiTreeNodeFlags _flagt = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
	_flagt |= ImGuiTreeNodeFlags_Framed;

	if (ImGui::TreeNodeEx("PRESETS", _flagt))
	{
		float _w100 = ofxImGuiSurfing::getWidgetsWidth(1);
		float _w50 = ofxImGuiSurfing::getWidgetsWidth(2);
		float _h = getWidgetsHeightRelative();

		//ImGui::Dummy(ImVec2(0, 2));

		// Index
		//ImGui::PushItemWidth(_w1 - 20);
		ofxImGuiSurfing::AddParameter(index);
		//ImGui::PopItemWidth();

		// Scrollable list
		if (!fileNames.empty())
		{
			int _i = index;

			//ImGui::PushItemWidth(WIDGET_PARAM_PADDING);
			//ImGui::PushItemWidth(_w1);
			if (ofxImGuiSurfing::VectorCombo(" ", &_i, fileNames))
			{
				ofLogNotice(__FUNCTION__) << "_i: " << ofToString(_i);

				if (_i < fileNames.size())
				{
					index = _i;

					//fileName = dir.getName(index);
					//ofLogNotice(__FUNCTION__) << "Combo select: " << _i;

					//if (dir.size() > 0 && index < dir.size())
					//{
					//	fileName = dir.getName(index);
					//	filePath = dir.getPath(index);
					//}
				}
			}
			//ImGui::PopItemWidth();
		}

		ImGui::PushButtonRepeat(true);
		{
			if (ImGui::Button("<", ImVec2(_w50, _h)))
			{
				doLoadPrevious();
			}
			ImGui::SameLine();
			if (ImGui::Button(">", ImVec2(_w50, _h)))
			{
				doLoadNext();
			}
		}
		ImGui::PopButtonRepeat();

		//static bool bEdit = false;
		//ofxImGuiSurfing::ToggleRoundedButton("Edit", &bEdit);
		//if (bEdit) {
		//	draw_ImGui_Editor();
		//}

		ImGui::TreePop();

		//ofxImGuiSurfing::AddToggleRoundedButton(bGui);//main
		ofxImGuiSurfing::AddToggleRoundedButton(bGui_Editor);
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
			if (nameRoot == "-1")
				n = "PARAMS";
			else {
				n = nameRoot;
				n += " PARAMS";
			}

			ImGuiWindowFlags flagsw;
			flagsw = ImGuiWindowFlags_None;
			flagsw |= ImGuiWindowFlags_AlwaysAutoResize;

			ImGui::PushID(("##" + n + params_Preset.getName()).c_str());
			{
				//IMGUI_SUGAR__WINDOWS_CONSTRAINTS;

				if (guiManager.beginWindow(n.c_str(), (bool*)&bGui_Parameters.get(), flagsw))
				{
					float _h = getWidgetsHeightUnit();
					float _w100 = getWidgetsWidth(1);
					float _w50 = getWidgetsWidth(2);

					//ImGui::Text(params_Preset.getName().c_str());

					//-

					// Minimize
					ofxImGuiSurfing::AddToggleRoundedButton(bMinimize_Params);

					//ofxImGuiSurfing::AddGroup(params_Preset, ImGuiTreeNodeFlags_DefaultOpen);
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

						ofxImGuiSurfing::AddSpacingSeparated();

						//TODO: not implemented yet
						if (bReset.getName() != "-1")
						{
							guiManager.Add(bReset, OFX_IM_BUTTON_SMALL);
						}

						//-

						guiManager.Add(bGui_Editor, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
						guiManager.Add(bGui_FloatingClicker, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
					}
				}
				guiManager.endWindow();
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

	if (ImGui::Button("Recall", ImVec2(_w50, _h)))
	{
		doRecallState();
	}
	ImGui::SameLine();
	if (ImGui::Button("Store", ImVec2(_w50, _h)))
	{
		doStoreState();
	}

	if (ImGui::Button("Reset", ImVec2(_w50, _h)))
	{
		doResetParams();
	}
	ImGui::SameLine();
	if (ImGui::Button("Random", ImVec2(_w50, _h)))
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
		draw_ImGui_Editor();

		//---

		draw_ImGui_FloatingClicker();

		//----

		draw_ImGui_Parameters();

		//--

//		// Player
		// for non instantiatex ImGui inside the addon
//#ifdef USE__OFX_SURFING_CONTROL__OFX_REMOTE_PARAMETERS__SERVER 
//		if (surfingPlayer.bGui)
//		{
//			ImGuiWindowFlags flagw = ImGuiWindowFlags_None;
//			if (guiManager.bAutoResize) flagw += ImGuiWindowFlags_AlwaysAutoResize;
//
//			if (guiManager.beginWindow(surfingPlayer.bGui, flagw))
//			{
//				surfingPlayer.draw();
//			}
//			guiManager.endWindow();
//		}
//#endif

	}
	guiManager.end();

	//--

	// Player
#ifdef USE__OFX_SURFING_PRESETS__OFX_SURFING_PLAYER 
	surfingPlayer.draw();
#endif

}
//--------------------------------------------------------------
void ofxSurfingPresets::keyReleased(ofKeyEventArgs &eventArgs)
{
	if (!bKeys) return;

	const int &key = eventArgs.key;
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
void ofxSurfingPresets::keyPressed(ofKeyEventArgs &eventArgs)
{
	if (!bKeys) return;

	const int &key = eventArgs.key;
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
	else if (key == OF_KEY_RETURN) { surfingPlayer.setPlayToggle(); }
#endif

	// Preset Index Selector by key numbers
	else
		for (int i = 0; i < keyCommands.size(); i++) {
			if (key == keyCommands[i]) {
				load(i);
				//continue;
				return;
			}
		}
	//for (int i = 0; i < NUM_KEY_COMMANDS; i++) {
	//	if (key == keyCommands[i]) {
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
//	if (bDISABLECALLBACKS) return;
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
void ofxSurfingPresets::Changed_Control(ofAbstractParameter &e)
{
	if (bDISABLECALLBACKS) return;

	{
		std::string name = e.getName();

		// Exclude debugs

		if (name != "exclude" && name != "exclude")
		{
			ofLogNotice(__FUNCTION__) << name << " : " << e;
		}

		//--

		// Preset Index

		if (name == index.getName())
		{
			index = ofClamp(index.get(), index.getMin(), index.getMax()); // clamp inside dir files amount limits

			// Changed?

			if (index.get() != index_PRE)
			{
				if (index_PRE != -1)
				{
					ofLogNotice(__FUNCTION__) << "\n\n  Changed \n  Preset Index : "
						<< ofToString(index_PRE) << " > " << ofToString(index)
						<< "      " << ofToString(keyCommands[index_PRE]) << " > " << ofToString(keyCommands[index])
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

					if (bf&&bt) {
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

		if (name == bSave.getName() && bSave)
		{
			bSave = false;
			save(filePath);
		}
		if (name == bLoad.getName() && bLoad)
		{
			bLoad = false;
			load(filePath);
		}
		if (name == bNewPreset.getName() && bNewPreset)
		{
			bNewPreset = false;
			doNewPreset();
		}
		if (name == bRefresh.getName() && bRefresh)
		{
			bRefresh = false;
			doRefreshFiles();
		}
		if (name == bSetPathPresets.getName() && bSetPathPresets)
		{
			setPath();
		}


		if (name == guiManager.bMinimize.getName())
		{
		}

		// workflow

		//if (name == bMODE_Active.getName())
		//{
		//	setActive(bMODE_Active);
		//}

		//if (name == bGui_Editor.getName())
		//{
		//	//TODO:
		//	//if (bGui_Editor)
		//	//{
		//	//	if (!bGui) bGui = true;
		//	//}
		//}
		//if (name == bGui.getName())
		//{
		//	if (!bGui_Editor && !bGui_FloatingClicker)
		//	{
		//		bGui_Editor = true;
		//	}
		//}
}
}

#ifdef USE_TOGGLE_TRIGGERS
//--------------------------------------------------------------
void ofxSurfingPresets::Changed_Params_PresetToggles(ofAbstractParameter &e)
{
	if (bDISABLECALLBACKS) return;

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
void ofxSurfingPresets::Changed_Params_Preset(ofAbstractParameter &e)
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
void ofxSurfingPresets::setPathGlobal(std::string s) // Must call before setup. disabled by default
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
	std::string ss = nameRoot + "_" + s;
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
		std::string ss = nameRoot + "_" + si;
		fileName = ss;
		filePath = path_Presets + "/" + ss + _ext;
		ofLogNotice(__FUNCTION__) << "Remove: " << filePath;
		ofFile::removeFile(filePath);

		//-

		// Recreate
		doRefreshFilesAndRename();
	}

	//TODO: 
	// workflow
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
void ofxSurfingPresets::doPopulatePresets()
{
	ofLogNotice(__FUNCTION__);

	doClearPresets(1);

	const int _max = AMOUNT_KIT_SIZE_DEFAULT;
	//const int _max = dir.size();

	for (int i = 0; i < _max - 1; i++)
	{
		index = i;
		doNewPreset();
		doSaveCurrent();
	}

	// workflow
	amntBtnsFloatClicker.setMax(_max);
	amntBtns.setMax(_max);
	amntBtnsFloatClicker.set(_max / 3);
	amntBtns.set(_max / 3);
}

//--------------------------------------------------------------
void ofxSurfingPresets::doPopulatePresetsRandomized()
{
	ofLogNotice(__FUNCTION__);

	doClearPresets(1);

	const int _max = AMOUNT_KIT_SIZE_DEFAULT;
	//const int _max = dir.size();

	for (int i = 0; i < _max - 1; i++)
	{
		index = i;
		doNewPreset();
		doSortParams(i);
		doSaveCurrent();
	}

	// workflow
	amntBtnsFloatClicker.setMax(_max);
	amntBtns.setMax(_max);
	amntBtnsFloatClicker.set(_max / 3);
	amntBtns.set(_max / 3);
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

//--------------------------------------------------------------
void ofxSurfingPresets::doCopyPreset()
{
	//TODO:

	//std::string ss = fileNames[index];
	//ss += "_";
	//ss += ofToString(dir.size());
	////ss += ofToString(index);

	//fileName = ss;
	//filePath = path_Presets + "/" + fileName + _ext;
	//ofLogNotice(__FUNCTION__) << filePath;

	//ofxSurfingHelpers::saveGroup(params_Preset, filePath);

	//doRefreshFiles();

	////TODO: fix name extension
	////TODO: set index to new one
	//
	//for (int i = 0; i < fileNames.size(); i++)
	//{
	//	auto _ss = ofSplitString(fileNames[i], ".");
	//	std::string _filename = "NoName";
	//	if (_ss.size() > 0) _filename = _ss[0];

	//	if (fileNames[i] == _filename)
	//	{
	//		ofLogNotice(__FUNCTION__) << "file " << "[" << ofToString(i) << "] " << dir.getName(i);
	//		index = i;
	//	}
	//}
}

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

		// workflow
		amntBtnsFloatClicker.setMax(dir.size());
		amntBtns.setMax(dir.size());
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
		std::string ss = nameRoot + "_" + si;
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

		//// workflow
		//amntBtnsFloatClicker.setMax(dir.size());
		//amntBtns.setMax(dir.size());
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

//--------------------------------------------------------------
void ofxSurfingPresets::doSortParams(int i) {
	//TODO: this is a workaround to autopopulate presets on ofxSurfingPresets...

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

		auto &p = params_Preset[i];

		//float v;
		//if (p.type() == typeid(ofParameter<float>).name())
		//{
		//	ofParameter<float> pr = p.cast<float>();
		//	pr = i;
		//}
		//else 
		if (p.type() == typeid(ofParameter<int>).name())
		{
			ofParameter<int> pr = p.cast<int>();
			pr = i;
		}
	}

	//bMustTrig = true;
	bIsRetrigged = true;
}

//--------------------------------------------------------------
void ofxSurfingPresets::doRandomizeParams() {
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

		auto &p = params_Preset[i];

		float v;

		if (p.type() == typeid(ofParameter<float>).name())
		{
			ofParameter<float> pr = p.cast<float>();
			pr = ofRandom(pr.getMin(), pr.getMax());
		}
		else if (p.type() == typeid(ofParameter<int>).name())
		{
			ofParameter<int> pr = p.cast<int>();
			pr = ofRandom(pr.getMin(), pr.getMax() + 1);
		}
	}

	//bMustTrig = true;
	bIsRetrigged = true;
}

//--------------------------------------------------------------
void ofxSurfingPresets::doResetParams() {
	ofLogNotice(__FUNCTION__);

	for (int i = 0; i < params_Preset.size(); i++)
	{
		auto &p = params_Preset[i];

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

	//bMustTrig = true;
	bIsRetrigged = true;
}
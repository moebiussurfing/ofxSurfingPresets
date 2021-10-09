#include "ofxSurfingPresets.h"


//--------------------------------------------------------------
ofxSurfingPresets::ofxSurfingPresets()
{
	// -> TODO: BUG?: 
	// it seems than requires to be false when using multi-context/instances
	// if is setted to true, sometimes it hangs and gui do not refresh/freezes.
	bAutoDraw = false;

	ofAddListener(ofEvents().update, this, &ofxSurfingPresets::update);
	ofAddListener(ofEvents().draw, this, &ofxSurfingPresets::draw, OF_EVENT_ORDER_AFTER_APP);

	path_Global = "ofxSurfingPresets/"; // this is to folder all files to avoid mixing with other addons data
	path_Presets = "ofxSurfingPresets/Presets";
	path_Params_Control = "ofxSurfingPresets_Settings.xml";
	path_filePreset = "Preset";

	DISABLE_Callbacks = true;

	setActive(true); // add key and mouse listeners

	//setup();
}

//--------------------------------------------------------------
ofxSurfingPresets::~ofxSurfingPresets()
{
	setActive(false);// remove keys and mouse listeners

	ofRemoveListener(ofEvents().update, this, &ofxSurfingPresets::update);
	ofRemoveListener(ofEvents().draw, this, &ofxSurfingPresets::draw, OF_EVENT_ORDER_AFTER_APP);

	// remove params callbacks listeners
	//ofRemoveListener(params.parameterChangedE(), this, &ofxSurfingPresets::Changed_Params);
	//ofRemoveListener(params_AppSettings.parameterChangedE(), this, &ofxSurfingPresets::Changed_AppSettings);
	ofRemoveListener(params_Control.parameterChangedE(), this, &ofxSurfingPresets::Changed_Control);

	//#ifdef INCLUDE__OFX_SURFING_PRESET__OFX_MIDI_PARAMS
	ofRemoveListener(params_PresetToggles.parameterChangedE(), this, &ofxSurfingPresets::Changed_Params_PresetToggles);
	//#endif

	ofxSurfingHelpers::saveGroup(params_AppSettings, path_Global + path_Params_Control);

	ofxSurfingHelpers::saveGroup(params_Preset, path_Global + path_filePreset + _ext);

	exit();
}

//#ifdef INCLUDE__OFX_SURFING_PRESET__OFX_MIDI_PARAMS
//--------------------------------------------------------------
void ofxSurfingPresets::refreshToggleNotes()
{
	if (DISABLE_Callbacks) return;

	for (int i = 0; i <= index.getMax() && i < notesIndex.size(); i++)
	{
		if (i == index.get()) notesIndex[i].set(true);
		else notesIndex[i].set(false);
	}
}
//#endif

//--------------------------------------------------------------
void ofxSurfingPresets::setup()
{
	// Log mode
	ofSetLogLevel("ofxSurfingPresets", OF_LOG_NOTICE);
	//ofSetLogLevel("ofxSurfingPresets", OF_LOG_SILENT);

	//--

	// Params 

	bGui.set("SURFING PRESETS", true);
	bGui_Editor.set("Editor", true);
	//bGui_Editor.set("Presets Editor", true);
	bCycled.set("Cycled", true);
	bAutoSave.set("AutoSave", true);
	bAutoSaveTimer.set("AutoSave Timed", false);
	bNewPreset.set("New", false);
	bSave.set("SAVE", false);
	bLoad.set("LOAD", false);
	bSetPathPresets.set("PATH", false);
	bRefresh.set("REFRESH", false);
	index.set("INDEX", 0, 0, 0);
	bGui_InnerClicker.set("Inner Clicker", false);
	bGui_FloatingClicker.set("Clicker", true);
	//bGui_FloatingClicker.set("Float Clicker", true);
	bGui_Parameters.set("Parameters", false);
	MODE_Active.set("Active", true);
	bDebug.set("Debug", true);
	bKeys.set("Keys", true);
	//bShowControl.set("Main Panel", true);
	//bHelp.set("HELP", false);	
	//MODE_App.set("APP MODE", 0, 0, NUM_MODES_APP - 1);
	//MODE_App_Name.set("", "");
	//MODE_App_Name.setSerializable(false);
	//ENABLE_Debug.set("DEBUG", true);

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

	//-

	// Params AppSettings

	params_AppSettings.clear();
	params_AppSettings.setName("AppSettings");
	params_AppSettings.add(bGui);
	params_AppSettings.add(bGui_Editor);
	params_AppSettings.add(bGui_Parameters);
	params_AppSettings.add(bGui_InnerClicker);
	params_AppSettings.add(bGui_FloatingClicker);
	//params_AppSettings.add(bShowControl);
	params_AppSettings.add(bAutoSave);
	params_AppSettings.add(bAutoSaveTimer);
	params_AppSettings.add(bCycled);
	params_AppSettings.add(bKeys);
	params_AppSettings.add(guiManager.bAutoResize);
	params_AppSettings.add(guiManager.bExtra);
	params_AppSettings.add(guiManager.bMinimize);
	params_AppSettings.add(MODE_Active);
	//params_AppSettings.add(bDebug);
	//params_AppSettings.add(MODE_App);
	//params_AppSettings.add(MODE_App_Name);
	//params_AppSettings.add(bHelp);
	//params_AppSettings.add(ENABLE_Debug);

	//-

	params_FloatClicker.clear();
	params_FloatClicker.setName("FLOAT CLICKER");
	//params_FloatClicker.add(bShowControl);
	params_FloatClicker.add(amntBtnsFloatClicker);
	params_FloatClicker.add(respBtnsFloatClicker);
	params_FloatClicker.add(bExtraFloatClicker);
	params_FloatClicker.add(bAutoResizeFloatClicker);
	params_AppSettings.add(params_FloatClicker);

	//-

	params_InnerClicker.clear();
	params_InnerClicker.setName("INNER CLICKER");
	params_InnerClicker.add(amntBtns);
	params_InnerClicker.add(respBtns);
	params_AppSettings.add(params_InnerClicker);

	// External extra or smooth params
	if (params_AppExtra.getName() != "-1") params_AppSettings.add(params_AppExtra);

	//ofAddListener(params_AppSettings.parameterChangedE(), this, &ofxSurfingPresets::Changed_AppSettings);

	// Exclude
	bSave.setSerializable(false);
	bNewPreset.setSerializable(false);
	bLoad.setSerializable(false);
	bSetPathPresets.setSerializable(false);
	bRefresh.setSerializable(false);
	MODE_Active.setSerializable(false);

	//-

	// All
	params.clear();
	params.setName("ALL PARAMS");
	params.add(params_Control);
	params.add(params_AppSettings);

	// All back
	//ofAddListener(params.parameterChangedE(), this, &ofxSurfingPresets::Changed_Params);

	//--

	// Gui
	//guiManager.setImGuiAutodraw(bAutoDraw);
	//guiManager.setup(); // this instantiates and configurates ofxImGui inside the class object.
	////guiManager.bAutoResize = false;

	guiManager.setSettingsPathLabel("ofxSurfingPresets");
	//guiManager.setAutoSaveSettings(true);
	guiManager.setup(IM_GUI_MODE_INSTANTIATED);

	//-

	// Files
	doRefreshFiles();

	//--

	//startup(); // called from AddGroup

	//--
}

//--------------------------------------------------------------
void ofxSurfingPresets::startup()
{
	ofLogNotice(__FUNCTION__);

	//-

	// Midi

	notesIndex.clear();
	params_PresetToggles.clear();
	for (int i = 0; i <= index.getMax(); i++)
	{
		string n = "Preset ";
		//n += ofToString(i < 10 ? "0" : "");
		n += ofToString(i);

		ofParameter<bool> b{ n, false };
		notesIndex.push_back(b);
		params_PresetToggles.add(b);
	}
	ofAddListener(params_PresetToggles.parameterChangedE(), this, &ofxSurfingPresets::Changed_Params_PresetToggles);

#ifdef INCLUDE__OFX_SURFING_PRESET__OFX_MIDI_PARAMS
	mMidiParams.connect();
	mMidiParams.add(params_Preset); // -> to control preset params
	mMidiParams.add(params_PresetToggles); // -> to select index prest by note/toggle and exclusive
	//mMidiParams.add(index);
#endif

#ifdef INCLUDE__OFX_SURFING_PRESET__OFX_PARAMETER_MIDI_SYNC
	mMidiParams.setup(params_Preset);
#endif

	//-

	DISABLE_Callbacks = false;

	//-

	// Settings
	ofxSurfingHelpers::loadGroup(params_AppSettings, path_Global + path_Params_Control);

	MODE_Active = true;

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
	index = 0;

	//-

	doRefreshFiles();
}

//--------------------------------------------------------------
void ofxSurfingPresets::update(ofEventArgs & args)
{
	// autosave
	//bAutoSave = false;
	if (bAutoSaveTimer && ofGetElapsedTimeMillis() - timerLast_Autosave > timeToAutosave)
	{
		DISABLE_Callbacks = true;

		ofxSurfingHelpers::saveGroup(params_AppSettings, path_Global + path_Params_Control);

		DISABLE_Callbacks = false;

		timerLast_Autosave = ofGetElapsedTimeMillis();
		ofLogNotice(__FUNCTION__) << "Autosaved DONE";
	}

	//--

#ifdef USE__OFX_SURFING_PRESETS__BASIC_SMOOTHER
	updateSmoother();
#endif
}

//--------------------------------------------------------------
void ofxSurfingPresets::draw(ofEventArgs & args)
{
	if (bGui)
	{
		draw_ImGui();

		//-

#ifdef INCLUDE__OFX_SURFING_PRESET__OFX_MIDI_PARAMS
		mMidiParams.draw();
#endif

#ifdef INCLUDE__OFX_SURFING_PRESET__OFX_PARAMETER_MIDI_SYNC
		mMidiParams.drawImGui();
#endif

	}
}

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
		ofxSurfingHelpers::ofxKuValueSmooth(v, pTar.get(), sp);
		pVal = v;
	}
	// Int
	else if (isInt) {
		ofParameter<int> pVal = ap.cast<int>();
		ofParameter<int> pTar = params_Preset.getInt(name); ;
		int v = pVal.get();
		ofxSurfingHelpers::ofxKuValueSmooth(v, pTar.get(), sp);
		pVal = v;
	}
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
void ofxSurfingPresets::draw_ImGui_EditorControl()
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

		// 1. Control

		//if (bShowContbShowControlrol)
		{
			ImGui::SetNextWindowSize(ImVec2(ww, hh), flagsCond);
			ImGui::SetNextWindowPos(ImVec2(xx, yy), flagsCond);

			n = "PRESETS EDITOR";
			//n = "PRESETS EDITOR " + params_Preset.getName();
			//n = params_Control.getName() + " |" + params_Preset.getName();
			//n = params_Control.getName();

			ImGui::PushID(("##" + n + params_Preset.getName()).c_str());
			{
				IMGUI_SUGAR__WINDOWS_CONSTRAINTS;

				guiManager.beginWindow(n.c_str(), (bool*)&bGui_Editor.get(), flagsw);
				{
					ImGui::Text(params_Preset.getName().c_str());

					guiManager.refreshLayout();
					_w100 = getWidgetsWidth(1);
					_w50 = getWidgetsWidth(2);
					_w33 = getWidgetsWidth(3);
					_w25 = getWidgetsWidth(4);
					_h = getWidgetsHeightUnit();

					//-

					ofxImGuiSurfing::AddToggleRoundedButton(guiManager.bMinimize);

					if (!guiManager.bMinimize)
					{
						//--

						//guiManager.refreshLayout();
						//_w1 = getWidgetsWidth(1);
						//_w2 = getWidgetsWidth(2);
						//_w3 = getWidgetsWidth(3);
						//_w4 = getWidgetsWidth(4);

						//--

						//// Textinput
						//{
						//	// input text
						//	static int keyboardFocus;
						//	//static char str0[128] = fileName.c_str();
						//	//std::string fileName = "Name";
						//	char *cstr = new char[fileName.length() + 1];
						//	strcpy(cstr, fileName.c_str());
						//	ImGui::InputText("Name", cstr, IM_ARRAYSIZE(cstr));
						//	//if (ImGui::IsItemClicked()) {
						//	//	keyboardFocus = 0;
						//	//}
						//	//if (ImGui::IsWindowFocused() && !ImGui::IsAnyItemActive()) {
						//	//	ImGui::SetKeyboardFocusHere(keyboardFocus);
						//	//}
						//}

						//ImGui::Text(path_Global.data());
						//ImGui::Text(filePath.data());

						string ss;
						if (dir.size() == 0) ss = "NO PRESETS";
						else ss = ofToString(index) + "/" + ofToString(index.getMax());
						//ss = ofToString(index) + "/" + ofToString(index.getMax());

						ImGui::Text(ss.data());
						if (guiManager.bMinimize) ImGui::Text(fileName.data()); // -> using text input below
						//if (guiManager.bExtra) ImGui::Text(path_Presets.data()); // -> show path

						//ImGui::Dummy(ImVec2(0, 1));
					}

					//--

					// 1. Scrollable list

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
							}
						}
						//ImGui::PopItemWidth();
					}

					//ImGui::Dummy(ImVec2(0, 2));

					//// index
					//if (!guiManager.bMinimize && guiManager.bExtra) ofxImGuiSurfing::AddIntStepped(index);

					// index
					//ImGui::PushItemWidth(_w1 - 20);
					//ImGui::PushItemWidth(100);
					//ImGui::PushItemWidth(WIDGET_PARAM_PADDING);
					ofxImGuiSurfing::AddParameter(index);
					//ImGui::PopItemWidth();

					// next
					if (guiManager.bMinimize)
					{
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
					}

					//--

					// TODO: copy this code to my ImGui hewlpers..

					if (bGui_InnerClicker)
					{
						draw_ImGui_MiniClicker();
					}

					//--

					//guiManager.Add(index, SurfingImGuiTypes::OFX_IM_DRAG); // crash
					//guiManager.Add(index, SurfingImGuiTypes::OFX_IM_DEFAULT);
					//guiManager.Add(index, SurfingImGuiTypes::OFX_IM_STEPPER);

					//// spinner
					//static int v = 1;
					//ImGuiInputTextFlags flags = 0;
					//Surfing::SpinInt("Index", &v, 1, 100, flags);

					if (!guiManager.bMinimize)
					{
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

						//ImGui::Dummy(ImVec2(0, 1));

						guiManager.Add(bSave, SurfingImGuiTypes::OFX_IM_BUTTON_SMALL, true, 2);
						guiManager.Add(bLoad, SurfingImGuiTypes::OFX_IM_BUTTON_SMALL, false, 2);

						guiManager.Add(bAutoSave, SurfingImGuiTypes::OFX_IM_TOGGLE_SMALL);
					}


					//if (guiManager.bMinimize)
					if (ImGui::Button("NEW", ImVec2(_w100, _h)))
					{
						//bNewPreset = true;
						doNewPreset();
					}

					//-

					// Minimize
					//ofxImGuiSurfing::AddToggleRoundedButton(guiManager.bMinimize);
					//ImGui::Dummy(ImVec2(0, 2));

					// Parameters
					ofxImGuiSurfing::AddToggleRoundedButton(bGui_Parameters);

					// Clicker
					ofxImGuiSurfing::AddToggleRoundedButton(bGui_FloatingClicker);

					if (!guiManager.bMinimize)
					{
						// Keys
						ofxImGuiSurfing::AddToggleRoundedButton(bKeys);

						// Midi
#ifdef INCLUDE__OFX_SURFING_PRESET__OFX_MIDI_PARAMS
						ofxImGuiSurfing::AddToggleRoundedButton(mMidiParams.bGui);
						//ofxImGuiSurfing::AddToggleRoundedButton(mMidiParams.bGui_Editor);
#endif

						bool bOpen = false;
						ImGuiTreeNodeFlags _flagt = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
						_flagt |= ImGuiTreeNodeFlags_Framed;

						if (ImGui::TreeNodeEx("TOOLS", _flagt))
						{
							//ImGui::SameLine();

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

								//if (ImGui::Button("DELETE", ImVec2(_w2, _h)))
								//{
								//	doDeletePreset(index);
								//}

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

								if (ImGui::Button("STORE", ImVec2(_w50, _h)))
								{
									doStoreState();
								}
								ImGui::SameLine();
								if (ImGui::Button("RECALL", ImVec2(_w50, _h)))
								{
									doRecallState();
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

								ImGui::TreePop();
							}

							//--

							if (ImGui::TreeNodeEx("KIT", _flagt))
							{
								guiManager.refreshLayout();
								_w100 = getWidgetsWidth(1);
								_w50 = getWidgetsWidth(2);
								_w33 = getWidgetsWidth(3);
								_w25 = getWidgetsWidth(4);
								_h = getWidgetsHeightUnit();

								guiManager.Add(bRefresh, SurfingImGuiTypes::OFX_IM_BUTTON_SMALL, true, 2);
								guiManager.Add(bSetPathPresets, SurfingImGuiTypes::OFX_IM_BUTTON_SMALL, false, 2);

								//TODO:
								//if (ImGui::Button("COPY", ImVec2(_w2, _h)))
								//{
								//	doCopyPreset();
								//}

								//TODO: show only on last preset
								//if (index == index.getMax())
								{
									//if (ImGui::Button("CLEAR", ImVec2(_w1, _h)))
									//{
									//	doClearPresets();
									//}

									if (ImGui::Button("CLEAR KIT", ImVec2(_w100, _h))) ImGui::OpenPopup("CLEAR KIT?");
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

							//ImGui::Dummy(ImVec2(0, 2));

							ImGui::TreePop();
						}
					}

					//-

					if (!guiManager.bMinimize)
					{
						ImGui::Dummy(ImVec2(0, 4));
						ofxImGuiSurfing::AddToggleRoundedButton(guiManager.bExtra);

						ImGui::Indent();

						if (guiManager.bExtra)
						{
							ofxImGuiSurfing::AddToggleRoundedButton(bGui_InnerClicker);
							//ofxImGuiSurfing::AddToggleRoundedButton(bGui_FloatingClicker);
							if (bGui_InnerClicker)
							{
								ImGui::Indent();

								//ofxImGuiSurfing::ToggleRoundedButton("Responsive", &respBtns);
								//if (respBtns) {
								//	ImGui::PushItemWidth(WIDGET_PARAM_PADDING);
								//	//ImGui::PushItemWidth(_w2 - 20);
								//	ImGui::SliderInt("Max Buttons", &amntBtns, 1, index.getMax() + 1);
								//	ImGui::PopItemWidth();
								//}

								ofxImGuiSurfing::AddToggleRoundedButton(respBtns);
								if (respBtns)
								{
									ImGui::PushItemWidth(WIDGET_PARAM_PADDING);
									ofxImGuiSurfing::AddIntStepped(amntBtns);
									ImGui::PopItemWidth();
								}

								ImGui::Unindent();
							}
							ofxImGuiSurfing::AddToggleRoundedButton(bCycled);
							//ofxImGuiSurfing::AddToggleRoundedButton(bKeys);
							ofxImGuiSurfing::AddToggleRoundedButton(bAutoSaveTimer);
							ofxImGuiSurfing::AddToggleRoundedButton(bAutoSave);
							//ofxImGuiSurfing::AddToggleRoundedButton(MODE_Active);
							//ofxImGuiSurfing::AddToggleRoundedButton(bDebug);
							ofxImGuiSurfing::AddToggleRoundedButton(guiManager.bAutoResize);

							bool bOpen = false;
							ImGuiTreeNodeFlags _flagt = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
							_flagt |= ImGuiTreeNodeFlags_Framed;
							if (ImGui::TreeNodeEx("PATHS", _flagt))
							{
								ImGui::Text(path_Presets.data()); // -> show path
								ImGui::TreePop();
							}
						}
						//ofxImGuiSurfing::ToggleRoundedButton("Debug", &bDebug);
					}

					//-

					// all

					//if (bDebug) {
					//	xx += ww + pad;
					//	ImGui::SetNextWindowSize(ImVec2(ww, hh), flagsCond);
					//	ImGui::SetNextWindowPos(ImVec2(xx, yy), flagsCond);
					//	n = "ofxSurfingPresets";
					//	guiManager.beginWindow(n.c_str(), &bOpen1, flagsw);
					//	{
					//		ImGuiTreeNodeFlags flagst;
					//		flagst = ImGuiTreeNodeFlags_None;
					//		flagst |= ImGuiTreeNodeFlags_DefaultOpen;
					//		flagst |= ImGuiTreeNodeFlags_Framed;
					//		ofxImGuiSurfing::AddGroup(params_AppSettings, flagst);
					//		ofxImGuiSurfing::AddGroup(params_Control, flagst);
					//	}
					//	guiManager.endWindow();
					//}

					//-

					// Extra

					if (!guiManager.bMinimize)
					{
						if (guiManager.bExtra)
						{
							//-

							// buttons selector

							if (ofxImGuiSurfing::filesPicker(path_Presets, nameSelected, index, { "json" }))
							{
								// buttons matrix

								//TODO. index back not working
								// this is a workaround
								// could fail on macOS/Linux -> requires fix paths slashes

								for (int i = 0; i < dir.size(); i++)
								{
									string si = ofToString(i);
									if (i < 10) si = "0" + si;
									string ss = nameRoot + "_" + si;
									fileName = ss;

									auto s0 = ofSplitString(nameSelected, "\\", true);
									string s1 = s0[s0.size() - 1]; // filename
									auto s = ofSplitString(s1, ".json");

									string _nameSelected = s[0];

									if (_nameSelected == fileName)
									{
										index = i;
									}
								}

								ofLogNotice(__FUNCTION__) << "Picked file " << nameSelected << " > " << index;

								//-

								//load(nameSelected);
							}

							//ImGui::Dummy(ImVec2(0, 2));
						}
					}

					//-

					// Extra panel
					if (guiManager.bExtra)
					{
						if (!guiManager.bMinimize)
						{
							guiManager.drawAdvanced();

							//if (bDebug) guiManager.drawAdvancedSubPanel();
							//if (bDebug) {
							//	ImGuiTreeNodeFlags flagst;
							//	flagst = ImGuiTreeNodeFlags_None;
							//	flagst |= ImGuiTreeNodeFlags_DefaultOpen;
							//	flagst |= ImGuiTreeNodeFlags_Framed;
							//	ofxImGuiSurfing::AddGroup(params, flagst);
							//}

							//ImGui::Unindent();
						}
					}
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

		std::string n;

		// 3. Floating Clicker
		ImGuiWindowFlags flagsw = ImGuiWindowFlags_None;
		if (bAutoResizeFloatClicker) flagsw |= ImGuiWindowFlags_AlwaysAutoResize;
		//if (guiManager.bAutoResize) flagsw |= ImGuiWindowFlags_AlwaysAutoResize;

		n = "PRESETS";
		//n = "PRESETS " + params_Preset.getName();

		ImGui::PushID(("##" + n + params_Preset.getName()).c_str());
		{
			IMGUI_SUGAR__WINDOWS_CONSTRAINTS;

			guiManager.beginWindow(n.c_str(), (bool*)&bGui_FloatingClicker.get(), flagsw);
			{
				ImGui::Text(params_Preset.getName().c_str());

				guiManager.refreshLayout();
				_w1 = getWidgetsWidth(1);
				_w2 = getWidgetsWidth(2);
				_w3 = getWidgetsWidth(3);
				_h = getWidgetsHeightUnit();

				// Clicker
				float sizey = ofxImGuiSurfing::getWidgetsHeightRelative() * 2;
				ofxImGuiSurfing::AddMatrixClicker(index, respBtnsFloatClicker, amntBtnsFloatClicker, true, sizey);

				// Parameters
				ofxImGuiSurfing::AddToggleRoundedButton(bGui_Parameters);

				// Editor
				ofxImGuiSurfing::AddToggleRoundedButton(bGui_Editor);

				//----

				// AppExtra
				if (params_AppExtra.getName() != "-1")
				{
					static ofParameter<bool> b = params_AppExtra.getBool("Smooth");
					static ofParameter<float> v = params_AppExtra.getFloat("Speed");
					//static ofParameter<bool> b = params_AppExtra.getBool("Smoother");
					//static ofParameter<bool> e = params_AppExtra.getBool("Enable");

					ofxImGuiSurfing::AddToggleRoundedButton(b);
					if (b)
					{
						ImGui::Indent();
						ImGui::PushItemWidth(_w2);
						ofxImGuiSurfing::AddParameter(v, "%.1f");
						ImGui::PopItemWidth();
						ImGui::Unindent();

						//ofxImGuiSurfing::AddParameter(e);
						//ofxImGuiSurfing::AddGroup(params_AppExtra, ImGuiTreeNodeFlags_None);
						//static ofParameterGroup g = params_AppExtra.getGroup("Smoother");
						//guiManager.AddGroup(g);
					}
				}

				//----

				// Floating
				ofxImGuiSurfing::AddToggleRoundedButton(bExtraFloatClicker);
				if (bExtraFloatClicker)
				{
					ImGui::Indent();

					//ofxImGuiSurfing::AddToggleRoundedButton(bShowControl);
					ofxImGuiSurfing::AddToggleRoundedButton(bAutoResizeFloatClicker);
					ofxImGuiSurfing::AddToggleRoundedButton(respBtnsFloatClicker);
					if (respBtnsFloatClicker)
					{
						//ImGui::PushItemWidth(WIDGET_PARAM_PADDING);
						//ImGui::PushItemWidth(_w3);
						guiManager.Add(amntBtnsFloatClicker, SurfingImGuiTypes::OFX_IM_STEPPER);
						//ofxImGuiSurfing::AddIntStepped(amntBtnsFloatClicker);
						//ofxImGuiSurfing::AddParameter(amntBtnsFloatClicker);
						//ImGui::PopItemWidth();
					}

					ImGui::Unindent();
				}
			}
			guiManager.endWindow();
		}
		ImGui::PopID();
	}
}


//--------------------------------------------------------------
void ofxSurfingPresets::draw_ImGui_MiniClicker() {
	static bool bOpen = true;
	ImGuiTreeNodeFlags _flagt = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
	_flagt |= ImGuiTreeNodeFlags_Framed;

	if (ImGui::TreeNodeEx("PRESETS", _flagt))
	{
		ofxImGuiSurfing::AddMatrixClicker(index, respBtns, amntBtns, true, WIDGETS_HEIGHT / 2);
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
		//	draw_ImGui_EditorControl();
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
			//xx += ww + pad;
			//ImGui::SetNextWindowSize(ImVec2(ww, hh), flagsCond);
			//ImGui::SetNextWindowPos(ImVec2(xx, yy), flagsCond);

			string n;
			n = "PARAMETERS";
			//n = "PARAMETERS " + params_Preset.getName();
			//n = params_Preset.getName();

			ImGuiWindowFlags flagsw;
			flagsw = ImGuiWindowFlags_None;
			flagsw |= ImGuiWindowFlags_AlwaysAutoResize;

			ImGui::PushID(("##" + n + params_Preset.getName()).c_str());
			{
				//IMGUI_SUGAR__WINDOWS_CONSTRAINTS;

				if (guiManager.beginWindow(n.c_str(), (bool*)&bGui_Parameters.get(), flagsw))
				{
					ImGui::Text(params_Preset.getName().c_str());

					ImGuiTreeNodeFlags flagst;
					flagst = ImGuiTreeNodeFlags_None;

					//if (!guiManager.bMinimize) flagst |= ImGuiTreeNodeFlags_DefaultOpen;

					flagst |= ImGuiTreeNodeFlags_DefaultOpen;
					//flagst |= ImGuiTreeNodeFlags_Framed;

					//TODO:
					ofxImGuiSurfing::AddGroup(params_Preset, flagst);
					//ofxImGuiSurfing::AddGroup(params_Preset);

					ofxImGuiSurfing::AddSpacingSeparated();

					//guiManager.endWindow();
					if (bReset.getName() != "-1")
					{
						guiManager.Add(bReset, OFX_IM_BUTTON_SMALL);
					}

					ofxImGuiSurfing::AddToggleRoundedButton(bGui_Editor);
					ofxImGuiSurfing::AddToggleRoundedButton(bGui_FloatingClicker);
					//if (bReset != nullptr) {
					//	if (ImGui::Button("Reset", (bool*)&bReset)) {
					//	}
					//}
				}
				guiManager.endWindow();
			}
			ImGui::PopID();
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingPresets::draw_ImGui()
{
	guiManager.begin();
	{
		draw_ImGui_EditorControl();

		//---

		draw_ImGui_FloatingClicker();

		//----

		draw_ImGui_Parameters();
	}
	guiManager.end();
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

	bool debug = false;
	if (debug)
	{
		ofLogNotice(__FUNCTION__) << "mod_COMMAND: " << (mod_COMMAND ? "ON" : "OFF");
		ofLogNotice(__FUNCTION__) << "mod_CONTROL: " << (mod_CONTROL ? "ON" : "OFF");
		ofLogNotice(__FUNCTION__) << "mod_ALT: " << (mod_ALT ? "ON" : "OFF");
		ofLogNotice(__FUNCTION__) << "mod_SHIFT: " << (mod_SHIFT ? "ON" : "OFF");
	}

	if (key == OF_KEY_LEFT) {
		doLoadPrevious();
	}
	else if (key == OF_KEY_RIGHT) {
		doLoadNext();
	}
	else if (key == OF_KEY_RETURN) {
		doSaveCurrent();
	}
	else if (key == OF_KEY_BACKSPACE) {
		doRandomizeParams();
	}

	else
		for (int i = 0; i < NUM_KEY_COMMANDS; i++) {
			if (key == keyCommands[i]) {
				load(i);
				//continue;
				return;
			}
		}
}

//--------------------------------------------------------------
void ofxSurfingPresets::addKeysListeners()
{
	ofAddListener(ofEvents().keyPressed, this, &ofxSurfingPresets::keyPressed);
}
//--------------------------------------------------------------
void ofxSurfingPresets::removeKeysListeners()
{
	ofRemoveListener(ofEvents().keyPressed, this, &ofxSurfingPresets::keyPressed);
}

//--------------------------------------------------------------
void ofxSurfingPresets::exit()
{
}

//--------------------------------------------------------------
void ofxSurfingPresets::setLogLevel(ofLogLevel level)
{
	ofSetLogLevel(__FUNCTION__, level);
}

//--------------------------------------------------------------
void ofxSurfingPresets::setActive(bool b)
{
	// disables all keys and mouse interaction listeners from the addon

	MODE_Active = b;

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
//	if (DISABLE_Callbacks) return;
//
//	{
//		string name = e.getName();
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
	if (DISABLE_Callbacks) return;

	{
		string name = e.getName();

		// Exclude debugs

		if (name != "exclude" && name != "exclude")
		{
			ofLogNotice(__FUNCTION__) << name << " : " << e;
		}

		// Index

		if (name == index.getName())
		{
			index = ofClamp(index.get(), index.getMin(), index.getMax());

			// Changed 

			if (index.get() != index_PRE)
			{
				ofLogNotice(__FUNCTION__) << "Changed index: " << ofToString(index_PRE) << " > " << ofToString(index);

				// Autosave

				if (bAutoSave)
				{
					if (dir.size() > 0 && index_PRE < dir.size())
					{
						//string _fileName = dir.getName(index_PRE);
						//string _filePath = dir.getPath(index_PRE);

						//int i = index_PRE;
						//string si = ofToString(i);
						//if (i < 10) si = "0" + si;
						//string ss = nameRoot + "_" + si;
						//fileName = ss;
						//filePath = path_Presets + "/" + ss + _ext;
						//ofLogNotice(__FUNCTION__) << filePath;

						int i = index_PRE;
						filePath = getFilepathForIndexPreset(i);

						save(filePath);
					}
					else
					{
						ofLogError(__FUNCTION__) << "File out of range";
					}
				}

				index_PRE = index;

				//-

				// Load

				ofLogNotice(__FUNCTION__) << "index: " << ofToString(index);

				if (dir.size() > 0 && index < dir.size())
				{
					//fileName = dir.getName(index);
					//filePath = dir.getPath(index);

					filePath = getFilepathForIndexPreset(index);
					load(filePath);

					//-

					//#ifdef INCLUDE__OFX_SURFING_PRESET__OFX_MIDI_PARAMS
					refreshToggleNotes();
					//#endif
				}
				else
				{
					ofLogError(__FUNCTION__) << "File out of range";
				}
			}
			else { bIsRetrigged = true; }
		}

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
		if (name == MODE_Active.getName())
		{
			setActive(MODE_Active);
		}
		if (name == bGui_Editor.getName())
		{
			if (bGui_Editor) {
				if (!bGui) bGui = true;
			}
		}
		if (name == bGui.getName()) // workflow
		{
			if (!bGui_Editor && !bGui_FloatingClicker)
			{
				bGui_Editor = true;
			}
		}
	}
}

////--------------------------------------------------------------
//void ofxSurfingPresets::Changed_AppSettings(ofAbstractParameter &e)
//{
//	if (DISABLE_Callbacks) return;
//
//	{
//		//string name = e.getName();
//
//		//// exclude debugs
//		//if (name != "exclude"
//		//	&& name != "exclude")
//		//{
//		//	ofLogNotice(__FUNCTION__) << name << " : " << e;
//		//}
//
//		//// control params
//		//if (name == "")
//		//{
//		//}
//		//else if (name == "APP MODE")
//		//{
//		//	switch (MODE_App)
//		//	{
//		//	case 0:
//		//		MODE_App_Name = "RUN";
//		//		//setActive(false);
//		//		break;
//		//	case 1:
//		//		MODE_App_Name = "EDIT";
//		//		//setActive(true);
//		//		break;
//		//	default:
//		//		MODE_App_Name = "UNKNOWN";
//		//		break;
//		//	}
//		//}
//
//		//// filter params
//		//if (name == "GUI POSITION")
//		//{
//		//	gui_Control.setPosition(Gui_Position.get().x, Gui_Position.get().y);
//		//}
//		//else if (name == "GUI")
//		//{
//		//}
//		//else if (name == "HELP")
//		//{
//		//}
//		//else if (name == "APP MODE")
//		//{
//		//}
//		//else if (name == "DEBUG")
//		//{
//		//}
//	}
//}

//#ifdef INCLUDE__OFX_SURFING_PRESET__OFX_MIDI_PARAMS
//--------------------------------------------------------------
void ofxSurfingPresets::Changed_Params_PresetToggles(ofAbstractParameter &e)
{
	if (DISABLE_Callbacks) return;

	string name = e.getName();

	for (int i = 0; i <= index.getMax() && i < notesIndex.size(); i++)
	{
		if (notesIndex[i].get() && name == notesIndex[i].getName())
		{
			index = i;
			//continue;
		}
	}

	// make exclusive
	for (int i = 0; i <= index.getMax() && i < notesIndex.size(); i++)
	{
		if (index != i && notesIndex[i].get())
		{
			notesIndex[i] = false;
		}
	}
}
//#endif

////--------------------------------------------------------------
//void ofxSurfingPresets::setKey_MODE_App(int k)
//{
//	key_MODE_App = k;
//}

//--------------------------------------------------------------
void ofxSurfingPresets::setPathGlobal(string s) // Must call before setup. disabled by default
{
	ofLogNotice(__FUNCTION__) << s;
	path_Global = s;

	ofxSurfingHelpers::CheckFolder(path_Global);
}
//--------------------------------------------------------------
void ofxSurfingPresets::setPathPresets(string s)
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
void ofxSurfingPresets::load(string path)
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
void ofxSurfingPresets::save(string path)
{
	ofLogNotice(__FUNCTION__) << path;
	ofxSurfingHelpers::saveGroup(params_Preset, path);

	//-

	// Callback
	// MODE A. it's important if this line is before or after ofSerialize
	ofLogVerbose(__FUNCTION__) << "DONE_save";
	DONE_save = true;

	//simple callback
	bIsDoneSave = true;
}

//--------------------------------------------------------------
void ofxSurfingPresets::doNewPreset()
{
	//string ss = fileNames[index];

	int _i = dir.size();
	string s = ofToString(_i);
	if (_i < 10) s = "0" + s;
	string ss = nameRoot + "_" + s;
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
	int diff = index.getMax() - (notesIndex.size() - 1);
	if (diff <= 0) return;

	for (int i = 0; i < diff; i++) {
		string n = "Preset ";
		n += ofToString(notesIndex.size() + i);
		ofParameter<bool> b{ n, false };
		notesIndex.push_back(b);
		params_PresetToggles.add(b);

#ifdef INCLUDE__OFX_SURFING_PRESET__OFX_MIDI_PARAMS
		mMidiParams.add(b);
#endif
	}

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
		string si = ofToString(i);
		if (i < 10) si = "0" + si;
		string ss = nameRoot + "_" + si;
		fileName = ss;
		filePath = path_Presets + "/" + ss + _ext;
		ofLogNotice(__FUNCTION__) << "Remove: " << filePath;
		ofFile::removeFile(filePath);

		//-

		// Recreate
		doRefreshFilesAndRename();
	}

	//TODO: set index to new one
	// should re sort and rename all the presets 
	// workflow

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
		//doSaveCurrent();
		doNewPreset();
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
		doRandomizeParams();
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

	//string ss = fileNames[index];
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
	//	string _filename = "NoName";
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

		string _name = "NoName"; // without ext
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

//#ifdef INCLUDE__OFX_SURFING_PRESET__OFX_MIDI_PARAMS
//	ofRemoveListener(params_PresetToggles.parameterChangedE(), this, &ofxSurfingPresets::Changed_Params_PresetToggles);
//	notesIndex.clear();
//	params_PresetToggles.clear();
//	for (int i = 0; i <= index.getMax(); i++)
//	{
//		string n = "Preset ";
//		//n += ofToString(i < 10 ? "0" : "");
//		n += ofToString(i);
//
//		ofParameter<bool> b{ n, false };
//		notesIndex.push_back(b);
//		params_PresetToggles.add(b);
//	}
//	ofAddListener(params_PresetToggles.parameterChangedE(), this, &ofxSurfingPresets::Changed_Params_PresetToggles);
//
//	mMidiParams.clear();
//	mMidiParams.add(params_Preset); // -> to control preset params
//	mMidiParams.add(params_PresetToggles); // -> to select index prest by note/toggle and exclusive
//#endif
}

//--------------------------------------------------------------
void ofxSurfingPresets::doRefreshFilesAndRename()
{
	// Load dragged images folder
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

		string si = ofToString(i);
		if (i < 10) si = "0" + si;
		string ss = nameRoot + "_" + si;
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

	bMustTrig = true;
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

	bMustTrig = true;
}
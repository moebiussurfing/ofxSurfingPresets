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

	//#ifdef USE_MIDI_PARAMS__SURFING_PRESETS
	ofRemoveListener(params_PresetToggles.parameterChangedE(), this, &ofxSurfingPresets::Changed_Params_PresetToggles);
	//#endif

	ofxSurfingHelpers::saveGroup(params_AppSettings, path_Global + path_Params_Control);

	ofxSurfingHelpers::saveGroup(params_Preset, path_Global + path_filePreset + _ext);

	exit();
}

//#ifdef USE_MIDI_PARAMS__SURFING_PRESETS
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
	// log mode
	ofSetLogLevel("ofxSurfingPresets", OF_LOG_NOTICE);
	//ofSetLogLevel("ofxSurfingPresets", OF_LOG_SILENT);

	//--

	// params 

	bGui.set("SURFING PRESETS", true);
	bGui_Editor.set("Presets Editor", true);
	bCycled.set("Cycled", true);
	bAutoSave.set("Auto Save", true);
	bAutoSaveTimer.set("Auto Save Timed", false);
	bSave.set("SAVE", false);
	bLoad.set("LOAD", false);
	bSetPathPresets.set("PATH", false);
	bRefresh.set("REFRESH", false);
	index.set("INDEX", 0, 0, 0);
	bGui_InnerClicker.set("Inner Clicker", false);
	bGui_FloatingClicker.set("Float Clicker", false);
	bGui_Parameters.set("Parameters", false);
	MODE_Active.set("Active", true);
	bDebug.set("Debug", true);
	bKeys.set("Keys", true);
	//bShowControl.set("Main Panel", true);
	//SHOW_Help.set("HELP", false);	
	//MODE_App.set("APP MODE", 0, 0, NUM_MODES_APP - 1);
	//MODE_App_Name.set("", "");
	//MODE_App_Name.setSerializable(false);
	//ENABLE_Debug.set("DEBUG", true);

	params_Control.clear();
	params_Control.setName("PRESETS CONTROL");
	params_Control.add(index);
	params_Control.add(bSave);
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

	// params AppSettings

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
	//params_AppSettings.add(SHOW_Help);
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

	//ofAddListener(params_AppSettings.parameterChangedE(), this, &ofxSurfingPresets::Changed_AppSettings);

	// exclude
	bSave.setSerializable(false);
	bLoad.setSerializable(false);
	bSetPathPresets.setSerializable(false);
	bRefresh.setSerializable(false);
	MODE_Active.setSerializable(false);

	//-

	// all
	params.clear();
	params.setName("ALL PARAMS");
	params.add(params_Control);
	params.add(params_AppSettings);

	// all back
	//ofAddListener(params.parameterChangedE(), this, &ofxSurfingPresets::Changed_Params);

	//--

	// gui
	guiManager.setImGuiAutodraw(bAutoDraw);
	guiManager.setup(); // this instantiates and configurates ofxImGui inside the class object.
	//guiManager.bAutoResize = false;

	//-

	// files
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

	// midi
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

#ifdef USE_MIDI_PARAMS__SURFING_PRESETS
	mMidiParams.connect();
	mMidiParams.add(params_Preset); // -> to control preset params
	mMidiParams.add(params_PresetToggles); // -> to select index prest by note/toggle and exclusive
	//mMidiParams.add(index);
#endif

	//-

	DISABLE_Callbacks = false;

	//-

	// settings
	ofxSurfingHelpers::loadGroup(params_AppSettings, path_Global + path_Params_Control);

	MODE_Active = true;

	// path for settings
	ofxSurfingHelpers::CheckFolder(path_Global);
	ofxSurfingHelpers::CheckFolder(path_Presets);

	// create first prest if folder it's empty
	if (dir.size() == 0)
	{
		doNewPreset();
	}

	ofxSurfingHelpers::loadGroup(params_Preset, path_Global + path_filePreset + _ext);

	// workflow
	// load first
	index = 0;

	//-

	// files
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
}

//--------------------------------------------------------------
void ofxSurfingPresets::draw(ofEventArgs & args)
{
	if (bGui)
	{
		draw_ImGui();

		//-

#ifdef USE_MIDI_PARAMS__SURFING_PRESETS
		mMidiParams.draw();
#endif
	}
}

//--------------------------------------------------------------
void ofxSurfingPresets::draw_ImGui_EditorControl()
{
	if (bGui_Editor)
	{
		// panels sizes
		float xx = 10;
		float yy = 10;
		float ww = PANEL_WIDGETS_WIDTH;
		float hh = PANEL_WIDGETS_HEIGHT * 2;
		int pad = 10;

		// widgets sizes
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

		// 1. control

		//if (bShowContbShowControlrol)
		{
			ImGui::SetNextWindowSize(ImVec2(ww, hh), flagsCond);
			ImGui::SetNextWindowPos(ImVec2(xx, yy), flagsCond);

			n = "PRESETS EDITOR " + params_Preset.getName();
			//n = params_Control.getName() + " |" + params_Preset.getName();
			//n = params_Control.getName();

			guiManager.beginWindow(n.c_str(), (bool*)&bGui_Editor.get(), flagsw);
			//guiManager.beginWindow(n.c_str(), &bOpen0, flagsw);
			{
				guiManager.refresh();
				_w100 = getWidgetsWidth(1);
				_w50 = getWidgetsWidth(2);
				_w33 = getWidgetsWidth(3);
				_w25 = getWidgetsWidth(4);

				//-

				ofxImGuiSurfing::AddToggleRoundedButton(guiManager.bMinimize);

				if (!guiManager.bMinimize)
				{
					//--

					//guiManager.refresh();
					//_w100 = getWidgetsWidth(1);
					//_w50 = getWidgetsWidth(2);
					//_w33 = getWidgetsWidth(3);
					//_w25 = getWidgetsWidth(4);

					//--

					//// textinput
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

				// 1. scrollable list

				if (!fileNames.empty())
				{
					int _i = index;
					//ImGui::PushItemWidth(WIDGET_PARAM_PADDING);
					ImGui::PushItemWidth(_w100 - 20);
					if (ofxImGuiSurfing::VectorCombo(" ", &_i, fileNames))
					{
						ofLogNotice(__FUNCTION__) << "_i: " << ofToString(_i);

						if (_i < fileNames.size())
						{
							index = _i;
						}
					}
					ImGui::PopItemWidth();
				}

				//ImGui::Dummy(ImVec2(0, 2));

				//// index
				//if (!guiManager.bMinimize && guiManager.bExtra) ofxImGuiSurfing::AddIntStepped(index);

				// index
				//ImGui::PushItemWidth(_w100 - 20);
				//ImGui::PushItemWidth(100);
				//ImGui::PushItemWidth(WIDGET_PARAM_PADDING);
				ofxImGuiSurfing::AddParameter(index);
				//ImGui::PopItemWidth();

				// next
				if (guiManager.bMinimize)
				{
					ImGui::PushButtonRepeat(true);
					{
						if (ImGui::Button("<", ImVec2(_w50, _h / 2)))
						{
							doLoadPrevious();
						}
						ImGui::SameLine();
						if (ImGui::Button(">", ImVec2(_w50, _h / 2)))
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
				if (ImGui::Button("NEW", ImVec2(_w100, _h / 2)))
				{
					doNewPreset();
				}

				//-

				// minimize
				//ofxImGuiSurfing::AddToggleRoundedButton(guiManager.bMinimize);
				//ImGui::Dummy(ImVec2(0, 2));

				// parameters
				ofxImGuiSurfing::AddToggleRoundedButton(bGui_Parameters);

				// clicker
				ofxImGuiSurfing::AddToggleRoundedButton(bGui_FloatingClicker);

				if (!guiManager.bMinimize)
				{
					// keys
					ofxImGuiSurfing::AddToggleRoundedButton(bKeys);
					// midi
#ifdef USE_MIDI_PARAMS__SURFING_PRESETS
					ofxImGuiSurfing::AddToggleRoundedButton(mMidiParams.bGui);
					//ofxImGuiSurfing::AddToggleRoundedButton(mMidiParams.bGui_Editor);
#endif

					bool bOpen = false;
					ImGuiTreeNodeFlags _flagt = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
					_flagt |= ImGuiTreeNodeFlags_Framed;

					if (ImGui::TreeNodeEx("TOOLS", _flagt))
					{
						guiManager.refresh();
						_w100 = getWidgetsWidth(1);
						_w50 = getWidgetsWidth(2);
						_w33 = getWidgetsWidth(3);
						_w25 = getWidgetsWidth(4);

						//ImGui::SameLine();

						if (ImGui::Button("NEW", ImVec2(_w50, _h / 2)))
						{
							doNewPreset();
						}
						ImGui::SameLine();
						if (ImGui::Button("DELETE LAST", ImVec2(_w50, _h / 2)))
						{
							doDeletePreset();
						}

						if (ImGui::Button("STORE", ImVec2(_w50, _h / 2)))
						{
							doStoreState();
						}
						ImGui::SameLine();
						if (ImGui::Button("RECALL", ImVec2(_w50, _h / 2)))
						{
							doRecallState();
						}

						if (ImGui::Button("RESET", ImVec2(_w50, _h / 2)))
						{
							doResetParams();
						}
						ImGui::SameLine();
						if (ImGui::Button("RANDOMiZE", ImVec2(_w50, _h / 2)))
						{
							doRandomizeParams();
						}

						guiManager.Add(bRefresh, SurfingImGuiTypes::OFX_IM_BUTTON_SMALL, true, 2);
						guiManager.Add(bSetPathPresets, SurfingImGuiTypes::OFX_IM_BUTTON_SMALL, false, 2);

						//TODO:
						//if (ImGui::Button("COPY", ImVec2(_w50, _h / 2)))
						//{
						//	doCopyPreset();
						//}

						//TODO: show only on last preset
						//if (index == index.getMax())
						{
							if (ImGui::Button("CLEAR KIT!", ImVec2(_w100, _h / 2)))
							{
								doClearPresets();
							}
							//ImGui::SameLine();

							if (ImGui::Button("POPULATE KIT", ImVec2(_w50, _h / 2)))
							{
								doPopulatePresets();
							}
							ImGui::SameLine();
							if (ImGui::Button("POPULATE RND", ImVec2(_w50, _h / 2)))
							{
								doPopulatePresetsRandomized();
							}
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
						ofxImGuiSurfing::AddToggleRoundedButton(guiManager.bAutoResize);
						ofxImGuiSurfing::AddToggleRoundedButton(bGui_InnerClicker);
						//ofxImGuiSurfing::AddToggleRoundedButton(bGui_FloatingClicker);
						if (bGui_InnerClicker)
						{
							ImGui::Indent();

							//ofxImGuiSurfing::ToggleRoundedButton("Responsive", &respBtns);
							//if (respBtns) {
							//	ImGui::PushItemWidth(WIDGET_PARAM_PADDING);
							//	//ImGui::PushItemWidth(_w50 - 20);
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
						ofxImGuiSurfing::AddToggleRoundedButton(bAutoSave);
						ofxImGuiSurfing::AddToggleRoundedButton(bAutoSaveTimer);
						//ofxImGuiSurfing::AddToggleRoundedButton(MODE_Active);
						//ofxImGuiSurfing::AddToggleRoundedButton(bDebug);

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

				// extra

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

				// extra panel
				if (guiManager.bExtra)
				{
					if (!guiManager.bMinimize)
					{
						ofxImGuiSurfing::AddToggleRoundedButton(guiManager.bAdvanced);
						//guiManager.bAdvanced = guiManager.bExtra; // link extra width advanced

						guiManager.drawAdvancedSubPanel();

						//if (bDebug) guiManager.drawAdvancedSubPanel();
						//if (bDebug) {
						//	ImGuiTreeNodeFlags flagst;
						//	flagst = ImGuiTreeNodeFlags_None;
						//	flagst |= ImGuiTreeNodeFlags_DefaultOpen;
						//	flagst |= ImGuiTreeNodeFlags_Framed;
						//	ofxImGuiSurfing::AddGroup(params, flagst);
						//}

						ImGui::Unindent();
					}
				}
			}
			guiManager.endWindow();
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingPresets::draw_ImGui_Floating()
{
	if (bGui_FloatingClicker)
	{
		// panels sizes
		float xx = 10;
		float yy = 10;
		float ww = PANEL_WIDGETS_WIDTH;
		float hh = PANEL_WIDGETS_HEIGHT * 2;
		int pad = 10;

		// widgets sizes
		float _w100;
		float _w50;
		float _w33;
		float _w25;
		float _h = WIDGETS_HEIGHT;

		std::string n;

		// 3. floating clicker
		ImGuiWindowFlags flagsw = ImGuiWindowFlags_None;
		if (bAutoResizeFloatClicker) flagsw |= ImGuiWindowFlags_AlwaysAutoResize;
		//if (guiManager.bAutoResize) flagsw |= ImGuiWindowFlags_AlwaysAutoResize;

		n = "PRESETS " + params_Preset.getName();
		//n = "PRESETS CLICKER";

		guiManager.beginWindow(n.c_str(), (bool*)&bGui_FloatingClicker.get(), flagsw);
		{
			guiManager.refresh();
			_w100 = getWidgetsWidth(1);
			_w50 = getWidgetsWidth(2);

			float sizey = ofxImGuiSurfing::getWidgetsHeightRelative() * 2;
			ofxImGuiSurfing::AddMatrixClicker(index, respBtnsFloatClicker, amntBtnsFloatClicker, true, sizey);

			ofxImGuiSurfing::AddToggleRoundedButton(bGui_Editor);

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
					ImGui::PushItemWidth(_w50);
					guiManager.Add(amntBtnsFloatClicker, SurfingImGuiTypes::OFX_IM_STEPPER);
					//ofxImGuiSurfing::AddIntStepped(amntBtnsFloatClicker);
					//ofxImGuiSurfing::AddParameter(amntBtnsFloatClicker);
					ImGui::PopItemWidth();
				}

				ImGui::Unindent();
			}
		}
		guiManager.endWindow();
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
		ofxImGuiSurfing::AddToggleRoundedButton(bGui_Editor);

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
		float _h = BUTTON_BIG_HEIGHT;

		//ImGui::Dummy(ImVec2(0, 2));

		// index
		//ImGui::PushItemWidth(_w100 - 20);
		ofxImGuiSurfing::AddParameter(index);
		//ImGui::PopItemWidth();

		// scrollable list
		if (!fileNames.empty())
		{
			int _i = index;

			//ImGui::PushItemWidth(WIDGET_PARAM_PADDING);
			ImGui::PushItemWidth(_w100 - 20);
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
			ImGui::PopItemWidth();
		}

		ImGui::PushButtonRepeat(true);
		{
			if (ImGui::Button("<", ImVec2(_w50, _h / 2)))
			{
				doLoadPrevious();
			}
			ImGui::SameLine();
			if (ImGui::Button(">", ImVec2(_w50, _h / 2)))
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
	// 2. preset params

	if (bGui_Parameters)
	{
		if (params_Preset.getName() != "-1")
		{
			//xx += ww + pad;
			//ImGui::SetNextWindowSize(ImVec2(ww, hh), flagsCond);
			//ImGui::SetNextWindowPos(ImVec2(xx, yy), flagsCond);

			string n;
			//n = "PARAMETERS";
			n = "PARAMETERS " + params_Preset.getName();
			//n = params_Preset.getName();

			ImGuiWindowFlags flagsw;
			flagsw = ImGuiWindowFlags_None;
			flagsw |= ImGuiWindowFlags_AlwaysAutoResize;

			//guiManager.beginWindow(n.c_str(), (bool*)&bGui_Parameters.get(), flagsw);
			//guiManager.beginWindow(n.c_str(), &bOpen1, flagsw);
			if (guiManager.beginWindow(n.c_str(), (bool*)&bGui_Parameters.get(), flagsw))
			{
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
	}
}

//--------------------------------------------------------------
void ofxSurfingPresets::draw_ImGui()
{
	guiManager.begin();
	{
		draw_ImGui_EditorControl();

		//---

		draw_ImGui_Floating();

		//----

		draw_ImGui_Parameters();
	}
	guiManager.end();

	//-

	//gui.draw();
}

//--------------------------------------------------------------
//void ofxSurfingPresets::keyPressed(int key)
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

////--------------------------------------------------------------
//void ofxSurfingPresets::windowResized(int w, int h)
//{
//	//gui_Control.setPosition(screenW * 0.5 - 200, screenH - 200);
//}

////keys
////--------------------------------------------------------------
//void ofxSurfingPresets::keyPressed(ofKeyEventArgs &eventArgs)
//{
//	const int &key = eventArgs.key;
//	ofLogNotice(__FUNCTION__) << (char)key << " [" << key << "]";
//
//	//modifiers
//	bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
//	bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
//	bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
//	bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);
//
//	bool debug = false;
//	if (debug)
//	{
//		ofLogNotice(__FUNCTION__) << "mod_COMMAND: " << (mod_COMMAND ? "ON" : "OFF");
//		ofLogNotice(__FUNCTION__) << "mod_CONTROL: " << (mod_CONTROL ? "ON" : "OFF");
//		ofLogNotice(__FUNCTION__) << "mod_ALT: " << (mod_ALT ? "ON" : "OFF");
//		ofLogNotice(__FUNCTION__) << "mod_SHIFT: " << (mod_SHIFT ? "ON" : "OFF");
//	}
//
//	//-
//
//	//disabler for all keys. (independent from MODE_Active)
//	if (bKeys)
//	{
//		//custom
//		if (key == ' ')
//		{
//		}
//		else if (key == ' ')
//		{
//		}
//
//		////custom with modifiers
//		//if (key == OF_KEY_UP && mod_ALT)
//		//{
//		//	ofLogNotice(__FUNCTION__) << "";
//		//}
//		//else if (key == OF_KEY_UP)
//		//{
//		//	ofLogNotice(__FUNCTION__) << "";
//		//}
//
//		////general
//		//if (key == key_MODE_App)
//		//{
//		//	int i = MODE_App;
//		//	i++;
//		//	MODE_App = i % NUM_MODES_APP;
//
//		//}
//		//else if (key == 'g')
//		//{
//		//	bGui = !bGui;
//		//}
//		//else if (key == 'h')
//		//{
//		//	SHOW_Help = !SHOW_Help;
//		//}
//		//else if (key == 'd')
//		//{
//		//	ENABLE_Debug = !ENABLE_Debug;
//		//}
//	}
//
//	//--
//
//	//key enabler
//	if (key == 'k')
//	{
//		bKeys = !bKeys;
//		ofLogNotice(__FUNCTION__) << "KEYS: " << (bKeys ? "ON" : "OFF");
//
//		if (!bKeys)
//		{
//			ofLogNotice(__FUNCTION__) << "ALL KEYS DISABLED. PRESS 'k' TO ENABLE GAIN!";
//		}
//		else
//		{
//			ofLogNotice(__FUNCTION__) << "KEYS ENABLED BACK";
//		}
//	}
//}

////--------------------------------------------------------------
//void ofxSurfingPresets::keyReleased(ofKeyEventArgs &eventArgs)
//{
//	const int &key = eventArgs.key;
//	ofLogNotice(__FUNCTION__) << (char)key << " [" << key << "]";
//
//	bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
//	bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
//	bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
//	bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);
//}

//// mouse
////--------------------------------------------------------------
//void ofxSurfingPresets::mouseDragged(ofMouseEventArgs &eventArgs)
//{
//	const int &x = eventArgs.x;
//	const int &y = eventArgs.y;
//	const int &button = eventArgs.button;
//	//ofLogNotice(__FUNCTION__) << "mouseDragged " << x << ", " << y << ", " << button;
//}
////--------------------------------------------------------------
//void ofxSurfingPresets::mousePressed(ofMouseEventArgs &eventArgs)
//{
//	const int &x = eventArgs.x;
//	const int &y = eventArgs.y;
//	const int &button = eventArgs.button;
//	//ofLogNotice(__FUNCTION__) << "mousePressed " << x << ", " << y << ", " << button;
//}
////--------------------------------------------------------------
//void ofxSurfingPresets::mouseReleased(ofMouseEventArgs &eventArgs)
//{
//	const int &x = eventArgs.x;
//	const int &y = eventArgs.y;
//	const int &button = eventArgs.button;
//	//ofLogNotice(__FUNCTION__) << "mouseReleased " << x << ", " << y << ", " << button;
//}
////--------------------------------------------------------------
//void ofxSurfingPresets::addMouseListeners()
//{
//	ofAddListener(ofEvents().mouseDragged, this, &ofxSurfingPresets::mouseDragged);
//	ofAddListener(ofEvents().mousePressed, this, &ofxSurfingPresets::mousePressed);
//	ofAddListener(ofEvents().mouseReleased, this, &ofxSurfingPresets::mouseReleased);
//}
////--------------------------------------------------------------
//void ofxSurfingPresets::removeMouseListeners()
//{
//	ofRemoveListener(ofEvents().keyPressed, this, &ofxSurfingPresets::keyPressed);
//}

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

		// exclude debugs

		if (name != "exclude" && name != "exclude")
		{
			ofLogNotice(__FUNCTION__) << name << " : " << e;
		}

		// index

		if (name == index.getName())
		{
			index = ofClamp(index.get(), index.getMin(), index.getMax());

			// changed 

			if (index.get() != index_PRE)
			{
				ofLogNotice(__FUNCTION__) << "Changed index: " << ofToString(index_PRE) << " > " << ofToString(index);

				// autosave

				if (bAutoSave)
				{
					if (dir.size() > 0 && index_PRE < dir.size())
					{
						//string _fileName = dir.getName(index_PRE);
						//string _filePath = dir.getPath(index_PRE);

						int i = index_PRE;
						string si = ofToString(i);
						if (i < 10) si = "0" + si;
						string ss = nameRoot + "_" + si;
						fileName = ss;
						filePath = path_Presets + "/" + ss + _ext;
						ofLogNotice(__FUNCTION__) << filePath;

						save(filePath);
					}
					else
					{
						ofLogError(__FUNCTION__) << "File out of range";
					}
				}

				index_PRE = index;

				//-

				// load

				ofLogNotice(__FUNCTION__) << "index: " << ofToString(index);

				if (dir.size() > 0 && index < dir.size())
				{
					//fileName = dir.getName(index);
					//filePath = dir.getPath(index);

					int i = index;
					string si = ofToString(i);
					if (i < 10) si = "0" + si;
					string ss = nameRoot + "_" + si;
					fileName = ss;
					filePath = path_Presets + "/" + ss + _ext;
					ofLogNotice(__FUNCTION__) << filePath;

					load(filePath);

					//-

					//#ifdef USE_MIDI_PARAMS__SURFING_PRESETS
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
		if (name == bLoad.getName() && bSave)
		{
			bLoad = false;
			load(filePath);
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

//#ifdef USE_MIDI_PARAMS__SURFING_PRESETS
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
void ofxSurfingPresets::setPathGlobal(string s) // must call before setup. disabled by default
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

	//simple callback
	bIsDoneLoad = true;
}

//--------------------------------------------------------------
void ofxSurfingPresets::doStoreState()
{
	ofLogNotice(__FUNCTION__) << path_Global + path_filePreset + _ext;
	ofxSurfingHelpers::saveGroup(params_Preset, path_Global + path_filePreset + _ext);

	DONE_save = true;

	//simple callback
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

	// callback
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

	// callback
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

#ifdef USE_MIDI_PARAMS__SURFING_PRESETS
		mMidiParams.add(b);
#endif
	}

}

//--------------------------------------------------------------
void ofxSurfingPresets::doDeletePreset()
{
	index = index.getMax();

	//int pre = index;

	ofFile::removeFile(filePath);
	ofLogNotice(__FUNCTION__) << filePath;
	doRefreshFiles();

	//TODO: set index to new one
	// should re sort and rename all the presets 
	// workflow

	//if (dir.size() > 0) index = pre;

	// load first file in dir
	//if (dir.size() > 0) index = 0;
	//else index = -1;

	// load last
	if (dir.size() > 0) index = index.getMax();
}

//--------------------------------------------------------------
void ofxSurfingPresets::doPopulatePresets()
{
	ofLogNotice(__FUNCTION__);

	doClearPresets(1);

	const int _max = AMOUNT_KIT_SIZE_DEFAULT;
	//const int _max = dir.size();

	for (int i = 0; i < _max; i++)
	{
		index = i;
		//doSaveCurrent();
		doNewPreset();
	}

	//workflow
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

	for (int i = 0; i < _max; i++)
	{
		index = i;
		doNewPreset();
		doRandomizeParams();
		doSaveCurrent();
	}

	//workflow
	amntBtnsFloatClicker.setMax(_max);
	amntBtns.setMax(_max);
	amntBtnsFloatClicker.set(_max / 3);
	amntBtns.set(_max / 3);
}

//--------------------------------------------------------------
void ofxSurfingPresets::doClearPresets(bool createOne)
{
	ofLogNotice(__FUNCTION__);

	// remove all files
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
void ofxSurfingPresets::doRefreshFiles()
{
	// load dragged images folder
	ofLogNotice(__FUNCTION__) << "list files " << path_Presets;

	dir.listDir(path_Presets);
	dir.allowExt("JSON");
	dir.allowExt("json");
	dir.sort();

	// log files on folder
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

		//workflow
		amntBtnsFloatClicker.setMax(dir.size());
		amntBtns.setMax(dir.size());
	}

	//-

//#ifdef USE_MIDI_PARAMS__SURFING_PRESETS
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
			pr = ofRandom(pr.getMin(), pr.getMax());
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
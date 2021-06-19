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
	path_Presets = "ofxSurfingPresets/presets";
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
	ofRemoveListener(params.parameterChangedE(), this, &ofxSurfingPresets::Changed_Params);
	ofRemoveListener(params_Internal.parameterChangedE(), this, &ofxSurfingPresets::Changed_Internal);
	ofRemoveListener(params_Control.parameterChangedE(), this, &ofxSurfingPresets::Changed_Control);

	ofxSurfingHelpers::saveGroup(params_Internal, path_Global + path_Params_Control);

	ofxSurfingHelpers::saveGroup(params_Preset, path_Global + path_filePreset + _ext);

	exit();
}

//--------------------------------------------------------------
void ofxSurfingPresets::setup()
{
	// log mode
	ofSetLogLevel("ofxSurfingPresets", OF_LOG_NOTICE);
	//ofSetLogLevel("ofxSurfingPresets", OF_LOG_SILENT);

	//--

	// params 
	bCycled.set("Cycled", false);
	bAutoSave.set("Auto Save", false);
	bSave.set("SAVE", false);
	bLoad.set("LOAD", false);
	bSetPathPresets.set("PATH", false);
	bRefresh.set("REFRESH", false);
	index.set("INDEX", 0, 0, 0);
	bShowParameters.set("PARAMETERS", true);
	MODE_Active.set("Active", true);
	bGui.set("GUI", true);
	bAutoSave.set("Auto Save", false);
	bDebug.set("Debug", true);

	params_Control.setName("PRESETS CONTROL");
	params_Control.add(bSave);
	params_Control.add(bLoad);
	params_Control.add(bSetPathPresets);
	params_Control.add(bShowParameters);
	params_Control.add(bRefresh);
	params_Control.add(bAutoSave);
	params_Control.add(bCycled);
	params_Control.add(index);

	ofAddListener(params_Control.parameterChangedE(), this, &ofxSurfingPresets::Changed_Control);

	//-

	// params
	ENABLE_keys.set("Keys", true);
	//SHOW_Help.set("HELP", false);	
	//MODE_App.set("APP MODE", 0, 0, NUM_MODES_APP - 1);
	//MODE_App_Name.set("", "");
	//MODE_App_Name.setSerializable(false);
	//ENABLE_Debug.set("DEBUG", true);

	// params internal
	params_Internal.setName("INTERNAL");
	params_Internal.add(bGui);
	params_Internal.add(bShowParameters);
	params_Internal.add(bAutoSave);
	params_Internal.add(bCycled);
	params_Internal.add(guiManager.bAutoResize);
	params_Internal.add(guiManager.bExtra);
	params_Internal.add(guiManager.bMinimize);
	params_Internal.add(ENABLE_keys);
	params_Internal.add(MODE_Active);
	//params_Internal.add(bDebug);
	//params_Internal.add(MODE_App);
	//params_Internal.add(MODE_App_Name);
	//params_Internal.add(SHOW_Help);
	//params_Internal.add(ENABLE_Debug);

	ofAddListener(params_Internal.parameterChangedE(), this, &ofxSurfingPresets::Changed_Internal);

	// exclude
	bSave.setSerializable(false);
	bLoad.setSerializable(false);
	bSetPathPresets.setSerializable(false);
	bRefresh.setSerializable(false);

	//-

	// all
	params.setName("ALL PARAMS");
	params.add(params_Control);
	params.add(params_Internal);

	// all back
	ofAddListener(params.parameterChangedE(), this, &ofxSurfingPresets::Changed_Params);

	//--

	// gui
	guiManager.setImGuiAutodraw(bAutoDraw);
	guiManager.setup(); // this instantiates and configurates ofxImGui inside the class object.
	//guiManager.bAutoResize = false;

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

	DISABLE_Callbacks = false;

	//-

	// settings
	ofxSurfingHelpers::loadGroup(params_Internal, path_Global + path_Params_Control);

	MODE_Active = true;

	// path for settings
	ofxSurfingHelpers::CheckFolder(path_Global);
	ofxSurfingHelpers::CheckFolder(path_Presets);

	// create first prest if folder it's empty
	if (dir.size() == 0)
	{
		doNewPreset();

		//string _name = params_Preset.getName() + "_00";
		//string _ext = ".json";
		//filePath = path_Presets + "/" + _name + _ext;

		//save(filePath);
		//doRefreshFiles();

		//index = 0;
	}

	ofxSurfingHelpers::loadGroup(params_Preset, path_Global + path_filePreset + _ext);

	// workflow
	// load first
	index = 0;

}

//--------------------------------------------------------------
void ofxSurfingPresets::update(ofEventArgs & args)
{
	// autosave
	//bAutoSave = false;
	if (bAutoSave && ofGetElapsedTimeMillis() - timerLast_Autosave > timeToAutosave)
	{
		DISABLE_Callbacks = true;

		ofxSurfingHelpers::saveGroup(params_Internal, path_Global + path_Params_Control);

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
	}
}

//--------------------------------------------------------------
void ofxSurfingPresets::draw_ImGui_Editor()
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

	//-

	std::string n;

	ImGuiWindowFlags flagsw = ImGuiWindowFlags_None;
	if (guiManager.bAutoResize) flagsw |= ImGuiWindowFlags_AlwaysAutoResize;

	ImGuiCond flagsCond = ImGuiCond_None;
	flagsCond |= ImGuiCond_FirstUseEver;

	//-

	{
		// control

		ImGui::SetNextWindowSize(ImVec2(ww, hh), flagsCond);
		ImGui::SetNextWindowPos(ImVec2(xx, yy), flagsCond);

		n = params_Control.getName();

		guiManager.beginWindow(n.c_str(), &bOpen0, flagsw);
		{
			widgetsManager.refreshPanelShape();
			_w100 = getWidgetsWidth(1);
			_w50 = getWidgetsWidth(2);
			_w33 = getWidgetsWidth(3);
			_w25 = getWidgetsWidth(4);

			//-

			if (!guiManager.bMinimize)
			{
				//--

				//widgetsManager.refreshPanelShape();
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

				string ss = ofToString(index) + "/" + ofToString(index.getMax());
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

			//ImGui::Dummy(ImVec2(0, 5));

			//// index
			//if (!guiManager.bMinimize && guiManager.bExtra) ofxImGuiSurfing::AddIntStepped(index);

			// index
			ofxImGuiSurfing::AddParameter(index);

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

			//widgetsManager.Add(index, SurfingWidgetTypes::IM_DRAG); // crash
			//widgetsManager.Add(index, SurfingWidgetTypes::IM_DEFAULT);
			//widgetsManager.Add(index, SurfingWidgetTypes::IM_STEPPER);

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

				widgetsManager.Add(bSave, SurfingWidgetTypes::IM_BUTTON_SMALL, true, 2);
				widgetsManager.Add(bLoad, SurfingWidgetTypes::IM_BUTTON_SMALL, false, 2);

				bool bOpen = false;
				ImGuiTreeNodeFlags _flagt = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
				_flagt |= ImGuiTreeNodeFlags_Framed;

				if (ImGui::TreeNodeEx("TOOLS", _flagt))
				{
					widgetsManager.refreshPanelShape();
					_w100 = getWidgetsWidth(1);
					_w50 = getWidgetsWidth(2);
					_w33 = getWidgetsWidth(3);
					_w25 = getWidgetsWidth(4);

					//ImGui::SameLine();

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

					if (ImGui::Button("NEW", ImVec2(_w50, _h / 2)))
					{
						doNewPreset();
					}
					ImGui::SameLine();
					if (ImGui::Button("CLEAR", ImVec2(_w50, _h / 2)))
					{
						doClearPresets();
					}

					//TODO:
					//if (ImGui::Button("COPY", ImVec2(_w50, _h / 2)))
					//{
					//	doCopyPreset();
					//}
					//TODO:
					//if (ImGui::Button("DELETE", ImVec2(_w50, _h / 2)))
					//{
					//	doDeletePreset();
					//}

					widgetsManager.Add(bRefresh, SurfingWidgetTypes::IM_BUTTON_SMALL, true, 2);
					widgetsManager.Add(bSetPathPresets, SurfingWidgetTypes::IM_BUTTON_SMALL, false, 2);

					//ImGui::Dummy(ImVec2(0, 2));

					ImGui::TreePop();
				}

			}

			//ImGui::Dummy(ImVec2(0, 2));

			//-

			// extra

			if (!guiManager.bMinimize)
			{
				if (guiManager.bExtra)
				{
					//-

					// buttons selector

					//TODO:
					//customize label

					int _i;
					if (ofxImGuiSurfing::filesPicker(path_Presets, nameSelected, { "json" })) {
						ofLogNotice(__FUNCTION__) << "Picked file " << nameSelected;
						load(nameSelected);
						int i = 0;
						for (auto n : fileNames) {
							string name = path_Presets + "/" + n;
							ofLogNotice(__FUNCTION__) << "name: " << name;
							if (nameSelected == name) {
								index.setWithoutEventNotifications(i);
							}
							i++;
						}
					}

					//ImGui::Dummy(ImVec2(0, 5));

					//-

					// debug
				}
			}

			//-

			// parameters
			ofxImGuiSurfing::AddToggleRoundedButton(bShowParameters);

			// minimize
			ofxImGuiSurfing::AddToggleRoundedButton(guiManager.bMinimize);
			//ImGui::Dummy(ImVec2(0, 2));

			//-

			if (!guiManager.bMinimize)
			{
				ofxImGuiSurfing::AddToggleRoundedButton(guiManager.bExtra);

				if (guiManager.bExtra) {
					ofxImGuiSurfing::AddToggleRoundedButton(bCycled);
					ofxImGuiSurfing::AddToggleRoundedButton(ENABLE_keys);
					//ofxImGuiSurfing::AddToggleRoundedButton(MODE_Active);
					ofxImGuiSurfing::AddToggleRoundedButton(bAutoSave);
					ImGui::Text(path_Presets.data()); // -> show path
					//ofxImGuiSurfing::AddToggleRoundedButton(bDebug);
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
			//		ofxImGuiSurfing::AddGroup(params_Internal, flagst);
			//		ofxImGuiSurfing::AddGroup(params_Control, flagst);
			//	}
			//	guiManager.endWindow();
			//}

			//-

			// extra panel
			guiManager.bAdvanced = guiManager.bExtra; // link extra width advanced
			if (!guiManager.bMinimize)
			{
				guiManager.drawAdvancedSubPanel();

				//if (bDebug) guiManager.drawAdvancedSubPanel();
				//if (bDebug) {
				//	ImGuiTreeNodeFlags flagst;
				//	flagst = ImGuiTreeNodeFlags_None;
				//	flagst |= ImGuiTreeNodeFlags_DefaultOpen;
				//	flagst |= ImGuiTreeNodeFlags_Framed;
				//	ofxImGuiSurfing::AddGroup(params, flagst);
				//}
			}
		}
		guiManager.endWindow();

		//----

		// preset params
		if (bShowParameters) {
			if (params_Preset.getName() != "-1")
			{
				xx += ww + pad;
				ImGui::SetNextWindowSize(ImVec2(ww, hh), flagsCond);
				ImGui::SetNextWindowPos(ImVec2(xx, yy), flagsCond);

				n = "PARAMETERS";
				//n = params_Preset.getName();
				//guiManager.beginWindow(n.c_str(), (bool*)&bShowParameters.get(), flagsw);
				guiManager.beginWindow(n.c_str(), &bOpen1, flagsw);
				{
					ImGuiTreeNodeFlags flagst;
					flagst = ImGuiTreeNodeFlags_None;
					flagst |= ImGuiTreeNodeFlags_DefaultOpen;
					flagst |= ImGuiTreeNodeFlags_Framed;

					ofxImGuiSurfing::AddGroup(params_Preset, flagst);
				}
				guiManager.endWindow();
			}
		}
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

		//ImGui::Dummy(ImVec2(0, 5));

		// index
		ofxImGuiSurfing::AddParameter(index);

		// scrollable list
		if (!fileNames.empty())
		{
			int _i = index;

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
		//	draw_ImGui_Editor();
		//}

		ImGui::TreePop();

		ofxImGuiSurfing::AddToggleRoundedButton(bGui);
	}
}

//--------------------------------------------------------------
void ofxSurfingPresets::draw_ImGui()
{
	guiManager.begin();
	{
		draw_ImGui_Editor();
	}
	guiManager.end();

	//-

	//gui.draw();
}

//--------------------------------------------------------------
//void ofxSurfingPresets::keyPressed(int key)
void ofxSurfingPresets::keyPressed(ofKeyEventArgs &eventArgs)
{
	const int &key = eventArgs.key;
	ofLogNotice(__FUNCTION__) << (char)key << " [" << key << "]";

	//modifiers
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

	if (!ENABLE_keys) return;

	if (key == OF_KEY_LEFT) {
		doLoadPrevious();
	}
	if (key == OF_KEY_RIGHT) {
		doLoadNext();
	}
	if (key == OF_KEY_RETURN) {
		doSaveCurrent();
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
//	if (ENABLE_keys)
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
//		ENABLE_keys = !ENABLE_keys;
//		ofLogNotice(__FUNCTION__) << "KEYS: " << (ENABLE_keys ? "ON" : "OFF");
//
//		if (!ENABLE_keys)
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

// all params
//--------------------------------------------------------------
void ofxSurfingPresets::Changed_Params(ofAbstractParameter &e)
{
	if (DISABLE_Callbacks) return;

	{
		string name = e.getName();

		//exclude debugs
		if (name != "exclude"
			&& name != "exclude")
		{
			ofLogNotice(__FUNCTION__) << "Changed_Params: " << name << " : " << e;

		}

		//params
		if (name == "")
		{
		}
	}
}

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
			}

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
			}
			else
			{
				ofLogError(__FUNCTION__) << "File out of range";
			}
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
	}
}

//--------------------------------------------------------------
void ofxSurfingPresets::Changed_Internal(ofAbstractParameter &e)
{
	if (DISABLE_Callbacks) return;

	{
		string name = e.getName();

		//exclude debugs
		if (name != "exclude"
			&& name != "exclude")
		{
			ofLogNotice(__FUNCTION__) << name << " : " << e;

		}

		if (name == "ACTIVE")
		{
			setActive(MODE_Active);
		}

		////control params
		//if (name == "")
		//{
		//}
		//else if (name == "APP MODE")
		//{
		//	switch (MODE_App)
		//	{
		//	case 0:
		//		MODE_App_Name = "RUN";
		//		//setActive(false);
		//		break;
		//	case 1:
		//		MODE_App_Name = "EDIT";
		//		//setActive(true);
		//		break;
		//	default:
		//		MODE_App_Name = "UNKNOWN";
		//		break;
		//	}
		//}

		////filter params
		//if (name == "GUI POSITION")
		//{
		//	gui_Control.setPosition(Gui_Position.get().x, Gui_Position.get().y);
		//}
		//else if (name == "GUI")
		//{
		//}
		//else if (name == "HELP")
		//{
		//}
		//else if (name == "APP MODE")
		//{
		//}
		//else if (name == "DEBUG")
		//{
		//}
	}
}

////--------------------------------------------------------------
//void ofxSurfingPresets::setKey_MODE_App(int k)
//{
//	key_MODE_App = k;
//}

//--------------------------------------------------------------
void ofxSurfingPresets::setPathGlobal(string s)//must call before setup. disabled by default
{
	ofLogNotice(__FUNCTION__) << s;
	path_Global = s;

	ofxSurfingHelpers::CheckFolder(path_Global);
}
//--------------------------------------------------------------
void ofxSurfingPresets::setPathPresets(string s)//must call before setup. disabled by default
{
	ofLogNotice(__FUNCTION__) << s;
	path_Presets = s;

	ofxSurfingHelpers::CheckFolder(path_Presets);
}

//--------------------------------------------------------------
void ofxSurfingPresets::doRecallState()
{
	ofLogNotice(__FUNCTION__) << path_Global + path_filePreset + _ext;
	ofxSurfingHelpers::loadGroup(params_Preset, path_Global + path_filePreset + _ext);

}

//--------------------------------------------------------------
void ofxSurfingPresets::doStoreState()
{
	ofLogNotice(__FUNCTION__) << path_Global + path_filePreset + _ext;
	ofxSurfingHelpers::saveGroup(params_Preset, path_Global + path_filePreset + _ext);

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
}

//--------------------------------------------------------------
void ofxSurfingPresets::load(string path)
{
	ofLogNotice(__FUNCTION__) << path;
	ofxSurfingHelpers::loadGroup(params_Preset, path);
}

//--------------------------------------------------------------
void ofxSurfingPresets::save(string path)
{
	ofLogNotice(__FUNCTION__) << path;
	ofxSurfingHelpers::saveGroup(params_Preset, path);
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
}

//--------------------------------------------------------------
void ofxSurfingPresets::doDeletePreset()
{
	int pre = index;

	ofFile::removeFile(filePath);
	ofLogNotice(__FUNCTION__) << filePath;
	doRefreshFiles();

	//TODO: set index to new one
	//should re sort and rename all the presets 
	// workflow

	//if (dir.size() > 0) index = pre;

	// load first file in dir
	if (dir.size() > 0) index = 0;
	//else index = -1;
}

//--------------------------------------------------------------
void ofxSurfingPresets::doClearPresets()
{
	ofLogNotice(__FUNCTION__);

	// remove all files
	for (int i = 0; i < dir.size(); i++)
	{
		ofFile file = dir[i];
		file.remove();
	}
	doRefreshFiles();

	doNewPreset();

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
	else {
		index.setMax(dir.size() - 1);
	}
}

//--------------------------------------------------------------
void ofxSurfingPresets::setPath()
{
	ofLogNotice(__FUNCTION__) << " SET PRESETS PATH";

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
}
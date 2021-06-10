#include "ofxSurfingPresets.h"

#pragma mark - OF

//--------------------------------------------------------------
ofxSurfingPresets::ofxSurfingPresets()
{
	ofAddListener(ofEvents().update, this, &ofxSurfingPresets::update);
	ofAddListener(ofEvents().draw, this, &ofxSurfingPresets::draw, OF_EVENT_ORDER_AFTER_APP);

	path_Global = "ofxSurfingPresets/";//this is to folder all files to avoid mixing with other addons data
	path_Presets = "ofxSurfingPresets/presets";//t
	path_Params_Control = "ofxSurfingPresets_Settings.xml";

	DISABLE_Callbacks = true;

	setActive(true);//add key and mouse listeners

	setup();
}

//--------------------------------------------------------------
ofxSurfingPresets::~ofxSurfingPresets()
{
	setActive(false);//remove keys and mouse listeners

	ofRemoveListener(ofEvents().update, this, &ofxSurfingPresets::update);
	ofRemoveListener(ofEvents().draw, this, &ofxSurfingPresets::draw, OF_EVENT_ORDER_AFTER_APP);

	//remove params callbacks listeners
	ofRemoveListener(params.parameterChangedE(), this, &ofxSurfingPresets::Changed_Params);
	ofRemoveListener(params_Internal.parameterChangedE(), this, &ofxSurfingPresets::Changed_Internal);
	ofRemoveListener(params_Control.parameterChangedE(), this, &ofxSurfingPresets::Changed_Control);

	ofxSurfingHelpers::saveGroup(params_Internal, path_Global + path_Params_Control);

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
	bSave.set("SAVE", false);
	bLoad.set("LOAD", false);
	bSetPathPresets.set("Set Path", false);
	bRefresh.set("Refresh", false);
	index.set("Index", 0, 0, 0);

	params_Control.setName("CONTROL");
	params_Control.add(bSave);
	params_Control.add(bLoad);
	params_Control.add(bSetPathPresets);
	params_Control.add(index);

	ofAddListener(params_Control.parameterChangedE(), this, &ofxSurfingPresets::Changed_Control);

	//-

	// params
	MODE_Active.set("ACTIVE", true);
	SHOW_Gui.set("GUI", true);
	SHOW_GuiPresets.set("PARAMETERS", true);
	ENABLE_AutoSave.set("AUTO SAVE", false);
	//ENABLE_keys.set("KEYS", true);
	//SHOW_Help.set("HELP", false);
	//MODE_App.set("APP MODE", 0, 0, NUM_MODES_APP - 1);
	//MODE_App_Name.set("", "");
	//MODE_App_Name.setSerializable(false);
	//ENABLE_Debug.set("DEBUG", true);

	// params internal
	params_Internal.setName("INTERNAL");
	params_Internal.add(SHOW_Gui);
	params_Internal.add(SHOW_GuiPresets);
	params_Internal.add(MODE_Active);
	params_Internal.add(ENABLE_AutoSave);
	params_Internal.add(guiManager.auto_resize);
	params_Internal.add(guiManager.bExtra);
	//params_Internal.add(ENABLE_keys);
	//params_Internal.add(MODE_App);
	//params_Internal.add(MODE_App_Name);
	//params_Internal.add(MODE_Active);
	//params_Internal.add(SHOW_Help);
	//params_Internal.add(ENABLE_Debug);
	//params_Internal.add(Gui_Position);

	ofAddListener(params_Internal.parameterChangedE(), this, &ofxSurfingPresets::Changed_Internal);

	//-

	// all
	params.setName("ALL PARAMS");
	params.add(params_Control);
	params.add(params_Internal);

	// all back
	ofAddListener(params.parameterChangedE(), this, &ofxSurfingPresets::Changed_Params);

	//--

	// gui
	guiManager.setup(); // this instantiates and configurates ofxImGui inside the class object.
	//guiManager.auto_resize = false;

	// files
	refreshFiles();

	//--

	startup();

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
}

//--------------------------------------------------------------
void ofxSurfingPresets::update(ofEventArgs & args)
{
	//--

	// autosave
	//ENABLE_AutoSave = false;
	if (ENABLE_AutoSave && ofGetElapsedTimeMillis() - timerLast_Autosave > timeToAutosave)
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
	if (SHOW_Gui)
	{
		draw_ImGui();
	}
}

//--------------------------------------------------------------
void ofxSurfingPresets::draw_ImGui()
{
	guiManager.begin();
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
		if (guiManager.auto_resize) flagsw |= ImGuiWindowFlags_AlwaysAutoResize;

		ImGuiCond flagsCond = ImGuiCond_None;
		flagsCond |= ImGuiCond_FirstUseEver;

		//-

		// windows
		{
			// control

			ImGui::SetNextWindowSize(ImVec2(ww, hh), flagsCond);
			ImGui::SetNextWindowPos(ImVec2(xx, yy), flagsCond);

			n = params_Control.getName();

			guiManager.beginWindow(n.c_str(), &bOpen0, flagsw);
			{
				widgetsManager.refreshPanelShape();

				_w100 = getImGui_WidgetWidth(1);
				_w50 = getImGui_WidgetWidth(2);
				_w33 = getImGui_WidgetWidth(3);
				_w25 = getImGui_WidgetWidth(4);

				//-

				// text
				//ImGui::Text(path_Global.data());
				//ImGui::Text(filePath.data());
				ImGui::Text(fileName.data());
				ImGui::Text(path_Presets.data());
				ImGui::Dummy(ImVec2(0, 1));

				// index
				ofxSurfing::AddParameter(index);
				ofxSurfing::AddIntStepped(index);
				//widgetsManager.Add(index, SurfingWidgetTypes::IM_DRAG); // crash
				//widgetsManager.Add(index, SurfingWidgetTypes::IM_DEFAULT);
				//widgetsManager.Add(index, SurfingWidgetTypes::IM_STEPPER);

				//// spinner
				//static int v = 1;
				//ImGuiInputTextFlags flags = 0;
				//Surfing::SpinInt("Index", &v, 1, 100, flags);

				ImGui::PushButtonRepeat(true);
				{
					if (ImGui::Button("<", ImVec2(_w50, _h)))
					{
						index--;
					}
					ImGui::SameLine();
					if (ImGui::Button(">", ImVec2(_w50, _h)))
					{
						index++;
					}
				}
				ImGui::PopButtonRepeat();
				ImGui::Dummy(ImVec2(0, 1));

				widgetsManager.Add(bSave, SurfingWidgetTypes::IM_BUTTON_SMALL, true, 2);
				widgetsManager.Add(bLoad, SurfingWidgetTypes::IM_BUTTON_SMALL, false, 2);
				widgetsManager.Add(bSetPathPresets, SurfingWidgetTypes::IM_BUTTON_SMALL, true, 2);
				widgetsManager.Add(bRefresh, SurfingWidgetTypes::IM_BUTTON_SMALL, false, 2);
				ImGui::Dummy(ImVec2(0, 5));

				//--

				// 1. scrollable list

				if (!fileNames.empty())
				{
					int _i = index;
					
					ImGui::PushItemWidth(_w100 - 20);
					if (ofxImGui::VectorCombo(" ", &_i, fileNames))
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
				ImGui::Dummy(ImVec2(0, 5));

				//-

				// extra

				ofxSurfing::AddToggleRoundedButton(guiManager.bExtra);
				//ImGui::Dummy(ImVec2(0, 5));

				if (guiManager.bExtra)
				{
					//-

					// buttons select
					int _i; //TODO:
					if (Surfing::SelectFile(path_Presets, nameSelected/*, _i*/)) {
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
					ImGui::Dummy(ImVec2(0, 5));

					//-

					// debug

					ofxSurfing::ToggleRoundedButton("Debug", &bDebug);

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

					//		ofxSurfing::AddGroup(params_Internal, flagst);
					//		ofxSurfing::AddGroup(params_Control, flagst);
					//	}
					//	guiManager.endWindow();
					//}

					//-

					// extra panel

					if (bDebug) guiManager.drawAdvancedSubPanel();

					if (bDebug) {
						ImGuiTreeNodeFlags flagst;
						flagst = ImGuiTreeNodeFlags_None;
						flagst |= ImGuiTreeNodeFlags_DefaultOpen;
						flagst |= ImGuiTreeNodeFlags_Framed;

						ofxSurfing::AddGroup(params, flagst);
					}
				}
			}
			guiManager.endWindow();

			//----

			// preset params

			if (params_Preset.getName() != "-1")
			{
				xx += ww + pad;
				ImGui::SetNextWindowSize(ImVec2(ww, hh), flagsCond);
				ImGui::SetNextWindowPos(ImVec2(xx, yy), flagsCond);

				n = params_Preset.getName();
				//guiManager.beginWindow(n.c_str(), (bool*)&SHOW_GuiPresets.get(), flagsw);
				guiManager.beginWindow(n.c_str(), &bOpen1, flagsw);
				{
					ImGuiTreeNodeFlags flagst;
					flagst = ImGuiTreeNodeFlags_None;
					flagst |= ImGuiTreeNodeFlags_DefaultOpen;
					flagst |= ImGuiTreeNodeFlags_Framed;

					ofxSurfing::AddGroup(params_Preset, flagst);
				}
				guiManager.endWindow();
			}
		}
	}
	guiManager.end();

	//-

	//gui.draw();
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
//		//	SHOW_Gui = !SHOW_Gui;
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

////--------------------------------------------------------------
//void ofxSurfingPresets::addKeysListeners()
//{
//	ofAddListener(ofEvents().keyPressed, this, &ofxSurfingPresets::keyPressed);
//}
////--------------------------------------------------------------
//void ofxSurfingPresets::removeKeysListeners()
//{
//	ofRemoveListener(ofEvents().keyPressed, this, &ofxSurfingPresets::keyPressed);
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
	//* disables all keys and mouse interaction listeners from the addon

	MODE_Active = b;

	//if (!b)
	//{
	//	removeKeysListeners();
	//	removeMouseListeners();
	//}
	//else
	//{
	//	addKeysListeners();
	//	addMouseListeners();
	//}
}

//--------------------------------------------------------------
void ofxSurfingPresets::setGuiVisible(bool b)
{
	SHOW_Gui = b;
}

// all params
//--------------------------------------------------------------
void ofxSurfingPresets::Changed_Params(ofAbstractParameter &e)
{
	if (!DISABLE_Callbacks)
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
	if (!DISABLE_Callbacks)
	{
		string name = e.getName();

		// exclude debugs
		if (name != "exclude" && name != "exclude")
		{
			ofLogNotice(__FUNCTION__) << name << " : " << e;
		}

		//if (name == "") {}

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
			refreshFiles();
		}
		if (name == index.getName())
		{
			index = ofClamp(index, index.getMin(), index.getMax());

			ofLogNotice(__FUNCTION__) << "index: " << ofToString(index);

			if (dir.size() > 0 && index < dir.size())
			{
				fileName = dir.getName(index);
				filePath = dir.getPath(index);

				load(filePath);
			}
			else
			{
				ofLogError(__FUNCTION__) << "File out of range";
			}
		}
	}
}

//--------------------------------------------------------------
void ofxSurfingPresets::Changed_Internal(ofAbstractParameter &e)
{
	if (!DISABLE_Callbacks)
	{
		string name = e.getName();

		//exclude debugs
		if (name != "exclude"
			&& name != "exclude")
		{
			ofLogNotice(__FUNCTION__) << name << " : " << e;

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
		//else if (name == "ACTIVE")
		//{
		//	setActive(MODE_Active);
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
	path_Global = s;

	ofxSurfingHelpers::CheckFolder(path_Global);
}
//--------------------------------------------------------------
void ofxSurfingPresets::setPathPresets(string s)//must call before setup. disabled by default
{
	path_Presets = s;

	ofxSurfingHelpers::CheckFolder(path_Presets);
}

//--------------------------------------------------------------
void ofxSurfingPresets::load(string path)
{
	ofxSurfingHelpers::loadGroup(params_Preset, path);
}
//--------------------------------------------------------------
void ofxSurfingPresets::save(string path)
{
	ofxSurfingHelpers::saveGroup(params_Preset, path);
}

//--------------------------------------------------------------
void ofxSurfingPresets::refreshFiles()
{
	// load dragged images folder
	ofLogNotice(__FUNCTION__) << "list files " << path_Presets;

	dir.listDir(path_Presets);
	//dir.allowExt("xml");
	//dir.allowExt("XML");
	dir.allowExt("JSON");
	dir.allowExt("json");
	dir.sort();

	// log files on folder
	fileNames.clear();
	for (int i = 0; i < dir.size(); i++)
	{
		ofLogNotice(__FUNCTION__) << "file " << "[" << ofToString(i) << "] " << dir.getName(i);

		fileNames.push_back(dir.getName(i));
	}

	index.setMin(0);
	index.setMax(dir.size() - 1);

	//// load first file in dir
	//if (dir.size() > 0) index = 0;
	//else index = -1;

	//if (dir.size() > 0 && index < dir.size() - 1)
	//{
	//	fileName = dir.getName(index);
	//	filePath = dir.getPath(index);
	//	buildFromImageFile(filePath, numColors);
	//}
}
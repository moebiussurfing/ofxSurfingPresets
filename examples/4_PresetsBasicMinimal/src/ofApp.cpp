#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetCircleResolution(200);

	// Params Group
	params.setName("myParams");
	params.add(size1.set("size1", 0.5f, 0.f, 0.1f));
	params.add(size2.set("size2", ofGetHeight() * 0.5f, 0, ofGetHeight() * 0.25f));
	params.add(rotation1.set("rotation1", 1, 0, 2));
	params.add(rotation2.set("rotation2", 180, 0, 360));
	params.add(indexColor.set("indexColor", 0, 0, 2));

	// Set the Presets Manager
	presetsManager.addGroup(params);

	// Gui init
	guiManager.setup();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	drawScene();

	//----

	guiManager.begin();
	{
		ImGuiCond flag = ImGuiCond_FirstUseEver;
		ImGui::SetNextWindowPos(ImVec2(ofGetWidth() - 200, 20), flag);

		if (guiManager.beginWindow(bGui_1))
		{
			guiManager.AddLabelBig("Simple and minimal / Inner Selectors");

			guiManager.Add(bClickerMinimal, OFX_IM_TOGGLE_BUTTON_ROUNDED_BIG);
			guiManager.Add(presetsManager.bGui_ClickerSimple, OFX_IM_TOGGLE_BUTTON_ROUNDED_BIG);
			
			guiManager.AddSpacingBigSeparated();
			
			//--

			guiManager.AddLabelBig("Presets Manager");

			guiManager.AddLabel("These are the full version windows of the add-on. On this example we are trying to preview the below minimal / simple versions! Go play with INNER SELECTOR sections below.", false, false);

			guiManager.Add(presetsManager.bGui, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);

			if (presetsManager.bGui) {
				guiManager.Indent();
				{
					guiManager.Add(presetsManager.bMinimize, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
					guiManager.Add(presetsManager.bAutoResize, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
					guiManager.AddSeparator();
				}
				guiManager.Unindent();
			}

			guiManager.AddSpacingBigSeparated();

			//--

			guiManager.AddLabelBig("Parameters"); // uppercased

			guiManager.Add(bParameters, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);

			if (bParameters) {
				guiManager.Indent(); // refreshes layout size checks
				guiManager.AddGroup(params);
				guiManager.Unindent(); // refreshes layout size checks
			}

			//--

			guiManager.endWindow();
		}

		//--

		if (guiManager.beginWindow(bGui_2))
		{
			guiManager.AddLabel("These are the minimal versions of the add-on: the minimal / simple versions! These are the bundle of widgets that we will like to easy integrate into our app GUIs.", false, false);

			guiManager.AddSpacingBigSeparated();

			if (bClickerMinimal)
			{
				guiManager.AddLabelBig("Clicker Minimal");

				presetsManager.draw_ImGui_ClickerMinimal(); // -> That bundle of widgets if what we want to use!

				guiManager.AddSpacingBigSeparated();
			}

			if (presetsManager.bGui_ClickerSimple)
			{
				guiManager.AddLabelBig("Clicker Simple");

				presetsManager.draw_ImGui_ClickerSimple(false); // -> That bundle of widgets if what we want to use!. First argument is to hide header.
				
				guiManager.AddSpacingBigSeparated();
			}

			guiManager.endWindow();
		}
	}
	guiManager.end();

	//----

	presetsManager.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == OF_KEY_F10) bGui_1 = !bGui_1;
}

//--------------------------------------------------------------
void ofApp::drawScene()
{
	// bg Color
	ofColor _colorBg = 64;//dark
	//ofColor _colorBg = 255;//white

	//--

	// The final variables to apply to draw the below Scene.
	// Notice that: 
	// If declared here, they must be static to maintain the previous frame setted values!
	static float _size1;
	static int _size2;
	static float _rotation1;
	static float _rotation2;

	//-

	//// A. No smooth. Raw values.
	//_size1 = size1;
	//_size2 = size2;
	//_rotation1 = rotation1;
	//_rotation2 = rotation2;

	// B. Addon Smoother
	_size1 = presetsManager.get(size1);
	_size2 = presetsManager.get(size2);
	_rotation1 = presetsManager.get(rotation1);
	_rotation2 = presetsManager.get(rotation2);

	//-

	// Process

	_size1 += 0.2;
	float _scale2 = _size1 / 5.f;
	int _rSz = _size2 + (ofGetHeight() * _scale2);
	float _rot = _rotation2 / 3.0;
	float _rat = ofMap(_rotation2, 0, 360, 1, 0.5f + _size1 * 0.4);
	float _rag = ofMap(_size1, 0, 1, 0, -30);

	//-

	// Shape Color

	ofColor _color;
	switch (indexColor)
	{
	case 0: _color = ofColor::black; break;
	case 1: _color = ofColor::orange; break;
	case 2: _color = ofColor::green; break;
	default: _color = ofColor::red; break;
	}

	int _alpha = ofMap(_size2, 0, size2.getMax(), 200, 245);
	ofColor c(_color.r, _color.g, _color.b, _alpha);

	//--

	// Draw

	ofClear(_colorBg);
	ofPushStyle();
	ofPushMatrix();
	{
		ofTranslate(ofGetWidth() * .5f, ofGetHeight() * .5f);
		ofRotateDeg(ofGetElapsedTimef() * TWO_PI);
		ofRotateZDeg(_rotation1 * 45);
		ofScale(1.3f);
		ofRotateDeg(_rot);

		ofSetColor(c);
		ofDrawRectangle(-_rSz * .5f, -_rSz * .5f, _rSz, _rSz);

		_rSz *= _rat;
		ofRotateDeg(_rot);
		ofTranslate(0, 0);
		_rot += _rag;

		ofSetColor(c);
		ofDrawRectangle(-_rSz * .5f, -_rSz * .5f, _rSz, _rSz);

		_rSz *= _rat;
		ofRotateDeg(_rot);
		ofTranslate(0, 0);
		_rot += _rag;

		ofSetColor(c);
		ofDrawRectangle(-_rSz * .5f, -_rSz * .5f, _rSz, _rSz);

		_rSz *= _rat;
		ofRotateDeg(_rot);
		ofTranslate(0, 0);
		_rot += _rag;

		ofSetColor(c);
		ofDrawRectangle(-_rSz * .5f, -_rSz * .5f, _rSz, _rSz);
	}
	ofPopMatrix();
	ofPopStyle();
}

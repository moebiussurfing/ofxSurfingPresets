#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetCircleResolution(200);

	// Group
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

		if (guiManager.beginWindow(bWindow))
		{
			guiManager.AddLabelBig("Presets Manager");//uppercased
			guiManager.Add(presetsManager.bGui, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
			guiManager.Indent();
			{
				guiManager.Add(presetsManager.bMinimize, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
				guiManager.Add(presetsManager.bAutoResize, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
				guiManager.Add(presetsManager.bLinkWindows, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
				guiManager.Add(presetsManager.bAlignWindows, OFX_IM_BUTTON_SMALL);
				guiManager.AddSpacing();
				guiManager.Add(bClickerMinimal, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
				guiManager.Add(presetsManager.bGui_ClickerSimple, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
				guiManager.AddSpacing();
				guiManager.Add(bParameters, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
			}
			guiManager.Unindent();

			guiManager.AddSeparator();

			if (bClickerMinimal)
			{
				guiManager.AddLabel("Clicker Minimal");
				presetsManager.draw_ImGui_ClickerMinimal();
				guiManager.AddSeparator();
			}
			if (presetsManager.bGui_ClickerSimple)
			{
				guiManager.AddLabel("Clicker Simple");
				presetsManager.draw_ImGui_ClickerSimple(false, false);
				guiManager.AddSeparator();
			}

			if (bParameters) guiManager.AddGroup(params);

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
	if (key == OF_KEY_F10) bWindow = !bWindow;
	if (key == OF_KEY_F11) presetsManager.doAlignWindowsOnce();
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
		ofTranslate(ofGetWidth()*.5f, ofGetHeight()*.5f);
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

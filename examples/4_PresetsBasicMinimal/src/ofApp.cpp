#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetCircleResolution(200);

	// Group
	params.setName("myParamsGroup");
	params.add(size1.set("size1", 0.5, 0, 1));
	params.add(size2.set("size2", ofGetHeight() * 0.5, 0, ofGetHeight() * 0.25));
	params.add(rotation1.set("rotation1", 1, 0, 2));
	params.add(rotation2.set("rotation2", 180, 0, 360));
	params.add(indexColor.set("indexColor", 0, 0, 2));

	presets.addGroup(params);

	setupImGui();
}
//--------------------------------------------------------------
void ofApp::setupImGui()
{
	guiManager.setup();
	//guiManager.addWindowSpecial(bWindow);
	guiManager.startup();
}

//--------------------------------------------------------------
void ofApp::update()
{
}

//--------------------------------------------------------------
void ofApp::draw()
{
	drawScene();

	//----

	guiManager.begin();
	{
		//if (bWindow)
			if (guiManager.beginWindow(bWindow))
			{
				guiManager.Add(guiManager.bAutoResize, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
				guiManager.Add(guiManager.bMinimize, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);

				ofxImGuiSurfing::AddSpacingBigSeparated();

				guiManager.Add(presets.bGui, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);

				ofxImGuiSurfing::AddSpacingBig();

				guiManager.Add(bClickerMinimal, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);
				guiManager.Add(presets.bGui_ClickerSimple, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);

				ofxImGuiSurfing::AddSpacingBig();

				guiManager.Add(bParameters, OFX_IM_TOGGLE_BUTTON_ROUNDED_MEDIUM);

				ofxImGuiSurfing::AddSpacingBigSeparated();

				if (bClickerMinimal) {
					presets.draw_ImGui_ClickerMinimal();
					ofxImGuiSurfing::AddSpacingBigSeparated();
				}

				if (presets.bGui_ClickerSimple) {
					presets.draw_ImGui_ClickerSimple(false, false);
					ofxImGuiSurfing::AddSpacingBigSeparated();
				}

				if (bParameters) guiManager.AddGroup(params);

				guiManager.endWindow();
			}
	}
	guiManager.end();

	//----

	presets.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == OF_KEY_F1) bWindow = !bWindow;
}

//--------------------------------------------------------------
void ofApp::drawScene()
{
	// The final variables to apply to draw the below Scene.
	// Notice that: 
	// If declared here, they must be static to mantain the previous frame setted values!
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
	_size1 = presets.get(size1);
	_size2 = presets.get(size2);
	_rotation1 = presets.get(rotation1);
	_rotation2 = presets.get(rotation2);

	//-

	// Preprocess
	_size1 += 0.2;
	float _scale2 = _size1 / 5.f;
	int _rSz = _size2 + (ofGetHeight() * _scale2);
	float _rot = _rotation2 / 3.0;
	float _rat = ofMap(_rotation2, 0, 360, 1, 0.5f + _size1 * 0.4);
	float _rag = ofMap(_size1, 0, 1, 0, -30);

	//-

	// Bg Color
	ofColor _colorBg = 32;//dark
	//ofColor _colorBg = 255;//white

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

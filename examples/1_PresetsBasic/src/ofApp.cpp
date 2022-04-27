#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetCircleResolution(200);

	// Group
	params.setName("paramsGroup");
	params.add(size1.set("size1", 0.5, 0, 1));
	params.add(size2.set("size2", ofGetHeight() * 0.5, 0, ofGetHeight() * 0.25));
	params.add(rotation1.set("rotation1", 1, 0, 2));
	params.add(rotation2.set("rotation2", 180, 0, 360));
	params.add(indexColor.set("indexColor", 0, 0, 2));

	presetsManager.addGroup(params);
}

//--------------------------------------------------------------
void ofApp::update()
{
}

//--------------------------------------------------------------
void ofApp::draw()
{
	drawScene();

	presetsManager.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == OF_KEY_F1) {
		presetsManager.setGuiVisibleToggle();
	}
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
	
	// These above variables are kind of aux variables 
	// and they are not required if you are not using the smoothing feature!
	// You can use the params variables (the ones added to the preset manager) directly:
	// size1, size2, rotation1, rotation2

	//// A. No smooth. Raw values.
	//_size1 = size1;
	//_size2 = size2;
	//_rotation1 = rotation1;
	//_rotation2 = rotation2;

	//-

	// B. Addon Smoother
	// Will smooth the variations of the variables.
	// Can be adjusted on the gui.
	_size1 = presetsManager.get(size1);
	_size2 = presetsManager.get(size2);
	_rotation1 = presetsManager.get(rotation1);
	_rotation2 = presetsManager.get(rotation2);

	//-

	// Some preprocess
	_size1 += 0.2;
	float _scale2 = _size1 / 5.f;
	int _rSz = _size2 + (ofGetHeight() * _scale2);
	float _rot = _rotation2 / 3.0;
	float _rat = ofMap(_rotation2, 0, 360, 1, 0.5f + _size1 * 0.4);
	float _rag = ofMap(_size1, 0, 1, 0, -30);

	//-

	// Bg Color
	ofColor _colorBg = 255;//white

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

	// Draw the Scene

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

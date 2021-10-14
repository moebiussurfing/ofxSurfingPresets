#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetCircleResolution(200);
	ofSetWindowPosition(1920, 25);

	// Group
	params.setName("paramsGroup");
	params.add(size1.set("size1", 0.5, 0, 1));
	params.add(size2.set("size2", ofGetHeight() * 0.5, 0, ofGetHeight() * 0.25));
	params.add(rotation1.set("rotation1", 1, 0, 2));
	params.add(rotation2.set("rotation2", 180, 0, 360));
	params.add(colorIndex.set("colorIndex", 0, 0, 2));

	presets.addGroup(params); 
	// -> WARNING! You must add only one group. Call this only once!
	// Put all your groups into one main container if you need it.

	//presets.setLogLevel(OF_LOG_SILENT); // disable log
}

//--------------------------------------------------------------
void ofApp::draw()
{
	drawScene();

	presets.draw(); // Gui
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

	//--

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

	//--

	// Preprocess
	_size1 += 0.2;
	float _scale2 = _size1 / 5.f;
	int _rSz = _size2 + (ofGetHeight() * _scale2);
	float _rot = _rotation2 / 3.0;
	float _rat = ofMap(_rotation2, 0, 360, 1, 0.5f + _size1 * 0.4);
	float _rag = ofMap(_size1, 0, 1, 0, -30);
	float _round = ofMap(rotation1, rotation1.getMin(), rotation1.getMax(), 0, 20, true);

	//-

	// Bg Color
	ofColor _colorBg = 255; // white

	// Shape Color
	ofColor _color;
	switch (colorIndex)
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
		ofDrawRectRounded(-_rSz * .5f, -_rSz * .5f, _rSz, _rSz, _round);

		_rSz *= _rat;
		ofRotateDeg(_rot);
		ofTranslate(0, 0);
		_rot += _rag;

		ofSetColor(c);
		ofDrawRectRounded(-_rSz * .5f, -_rSz * .5f, _rSz, _rSz, _round);

		_rSz *= _rat;
		ofRotateDeg(_rot);
		ofTranslate(0, 0);
		_rot += _rag;

		ofSetColor(c);
		ofDrawRectRounded(-_rSz * .5f, -_rSz * .5f, _rSz, _rSz, _round);

		_rSz *= _rat;
		ofRotateDeg(_rot);
		ofTranslate(0, 0);
		_rot += _rag;

		ofSetColor(c);
		ofDrawRectRounded(-_rSz * .5f, -_rSz * .5f, _rSz, _rSz, _round);
	}
	ofPopMatrix();
	ofPopStyle();
}

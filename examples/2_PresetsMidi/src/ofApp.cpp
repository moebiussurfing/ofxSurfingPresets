#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetCircleResolution(200);
	//ofSetWindowPosition(1920, 25);

	// Group
	params.setName("paramsGroup");
	params.add(size1.set("size1", 0.5, 0, 1));
	params.add(size2.set("size2", ofGetHeight() * 0.5, 0, ofGetHeight() * 0.25));
	params.add(rotation1.set("rotation1", 1, 0, 2));
	params.add(rotation2.set("rotation2", 180, 0, 360));
	params.add(indexColor.set("indexColor", 0, 0, 2));

	presets.addGroup(params);

	// Extra Controls for this app
	presets.addParamsAppExtra(params_Smooth);
}

//--------------------------------------------------------------
void ofApp::draw()
{
	drawScene();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == 'g') {
		presets.bGui = !presets.bGui;
	}
	if (key == ' ') {
		presets.doLoadNext();
	}
}

//--------------------------------------------------------------
void ofApp::drawScene()
{
	// Final variables to apply to the Scene below.
	// Notice that: If declared here, they must be static to mantain the previous frame values.
	static float _size1;
	static int _size2;
	static float _rotation1;
	static float _rotation2;

	//--

	// No smooth. Raw values.
	if (!bSmooth.get())
	{
		_size1 = size1;
		_size2 = size2;
		_rotation1 = rotation1;
		_rotation2 = rotation2;
	}
	// Smoothed
	else
	{
		static const float MAX_CLAMP_SMOOTH = 0.85f;
		float sp = ofMap(smoothSpeed, 1, 0, 0.1f, MAX_CLAMP_SMOOTH);
		ofxSurfingHelpers::ofxKuValueSmooth(_size1, size1.get(), sp);
		ofxSurfingHelpers::ofxKuValueSmooth(_size2, size2.get(), sp);
		ofxSurfingHelpers::ofxKuValueSmooth(_rotation1, rotation1.get(), sp);
		ofxSurfingHelpers::ofxKuValueSmooth(_rotation2, rotation2.get(), sp);
	}

	//-

	// Pre process
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

	//--

	// Draw

	ofClear(_colorBg);

	ofPushStyle();
	ofPushMatrix();
	{
		ofTranslate(ofGetWidth()*.5f, ofGetHeight()*.5f);
		ofRotateDeg(ofGetElapsedTimef() * TWO_PI);

		ofRotateDeg(ofGetElapsedTimef() * TWO_PI);
		ofRotateZDeg(_rotation1 * 45);
		ofScale(1.3f);
		ofRotateDeg(_rot);

		ofSetColor(_color.r * 255, _color.g * 255, _color.b * 255, _color.a * _alpha);
		ofDrawRectangle(-_rSz * .5f, -_rSz * .5f, _rSz, _rSz);

		_rSz *= _rat;
		ofRotateDeg(_rot);
		ofTranslate(0, 0);
		_rot += _rag;

		ofSetColor(_color.r * 255, _color.g * 255, _color.b * 255, _color.a * _alpha);
		ofDrawRectangle(-_rSz * .5f, -_rSz * .5f, _rSz, _rSz);

		_rSz *= _rat;
		ofRotateDeg(_rot);
		ofTranslate(0, 0);
		_rot += _rag;

		ofSetColor(_color.r * 255, _color.g * 255, _color.b * 255, _color.a * _alpha);
		ofDrawRectangle(-_rSz * .5f, -_rSz * .5f, _rSz, _rSz);

		_rSz *= _rat;
		ofRotateDeg(_rot);
		ofTranslate(0, 0);
		_rot += _rag;

		ofSetColor(_color.r * 255, _color.g * 255, _color.b * 255, _color.a * _alpha);
		ofDrawRectangle(-_rSz * .5f, -_rSz * .5f, _rSz, _rSz);
	}
	ofPopMatrix();
	ofPopStyle();
}

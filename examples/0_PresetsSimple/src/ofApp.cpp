#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetWindowPosition(-1920, 25);

	// Group
	params.setName("myScene");
	params.add(size1.set("size1", 0.5, 0, 1));
	params.add(size2.set("size2", ofGetHeight() /2, 0, ofGetHeight()/4));
	params.add(rotation1.set("rotation1", 1, 0, 2));
	params.add(rotation2.set("rotation2", 180, 0, 360));
	params.add(indexColor.set("indexColor", 0, 0, 2));

	presetsManager.addGroup(params);
}

//--------------------------------------------------------------
void ofApp::draw()
{
	drawScene(); // Will use the presets params!

	presetsManager.draw(); // Draw Gui
}

//--------------------------------------------------------------
void ofApp::drawScene()
{
	// Bg Color
	ofColor _colorBg = 24;//dark

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

	int _alpha = ofMap(size2, 0, size2.getMax(), 200, 245);
	ofColor c(_color.r, _color.g, _color.b, _alpha);

	//--
	 
	// Some pre process
	float _scale2 = size1 / 5.f;
	int _rSz = size2 + (ofGetHeight() * _scale2);
	float _rot = rotation2 / 3.0;
	float _rat = ofMap(rotation2, 0, 360, 1, 0.5f + size1 * 0.4);
	float _rag = ofMap(size1, 0, 1, 0, -30);

	//--

	// Draw the Scene

	ofClear(_colorBg);

	ofPushStyle();
	ofPushMatrix();
	{
		ofTranslate(ofGetWidth() * .5f, ofGetHeight() * .5f);
		ofRotateDeg(ofGetElapsedTimef() * TWO_PI);
		ofRotateZDeg(rotation1 * 45);
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

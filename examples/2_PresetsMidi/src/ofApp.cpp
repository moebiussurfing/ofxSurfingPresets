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
	params.add(indexColor.set("indexColor", 0, 0, 2));

	presets.addGroup(params);
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
	float _size1 = size1 + 0.2;
	float _size2 = size2;
	float _rotation1 = (rotation1);
	float _rotation2 = (rotation2);
	float _scale2 = _size1 / 5.f;
	int _rSz = _size2 + (ofGetHeight() * _scale2);
	float _rot = _rotation2 / 3.0;
	float _rat = ofMap(_rotation2, 0, 360, 1, 0.5f + _size1 * 0.4);
	float _rag = ofMap(_size1, 0, 1, 0, -30);

	//-

	ofColor _colorBg = 255;

	ofColor _color;
	switch (indexColor)
	{
	case 0: _color = ofColor::black; break;
	case 1: _color = ofColor::orange; break;
	case 2: _color = ofColor::green; break;
	}
	int _alpha = ofMap(_size2, 0, size2.getMax(), 200, 245);

	//--

	ofClear(_colorBg);

	ofPushStyle();
	ofPushMatrix();

	ofTranslate(ofGetWidth()*.5f, ofGetHeight()*.5f);
	ofRotateDeg(ofGetElapsedTimef() * TWO_PI);

	ofRotateDeg(ofGetElapsedTimef() * TWO_PI);
	ofRotateZDeg(_rotation1 * 45);
	ofScale(1.3);
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

	ofPopMatrix();
	ofPopStyle();
}

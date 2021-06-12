#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	// group
	params.setName("paramsGroup");
	params.add(fill.set("fill", true));
	params.add(color.set("color", ofColor(0, 255), ofColor(0, 0), ofColor(255, 255)));
	params.add(lineWidth.set("lineWidth", 1, 0.1, 10));
	params.add(shapeType.set("shapeType", 1, 1, 2));
	params.add(size.set("size", 100, 5, 200));
	params.add(amount.set("amount", 10, 1, 24));
	params.add(separation.set("separation", 10, 1, 100));

	presets.addGroup(params);
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofBackground(128);

	// scene draw using parameters
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(500, 50);
	ofSetColor(color.get());
	ofSetLineWidth(lineWidth);
	if (fill) ofFill();
	else ofNoFill();
	for (int i = 0; i < amount; ++i)
	{
		if (shapeType == 1) ofDrawRectangle(0, 0, size, size);
		else if (shapeType == 2) ofDrawCircle(0, 0, size);
		ofTranslate(separation, separation);
	}
	ofPopMatrix();
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::exit() {
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	if (key == 'g') {
		presets.bGui = !presets.bGui;
	}
}

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetCircleResolution(200);

	// group
	params.setName("paramsGroup");
	params.add(size1.set("size1", 1, 0, 2));
	params.add(size2.set("size2", ofGetHeight() * 0.5, 0, ofGetHeight() * 0.25));
	params.add(rotation.set("rotation", 1, 0, 2));
	params.add(rotationOffset.set("rotationOffset", 180, 0, 360));

	presets.setImGuiAutodraw(true); // -> required when using only one ImGui instance inside the add-ons of your ofApp
	presets.addGroup(params);
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw()
{
	drawScene();
}

//--------------------------------------------------------------
void ofApp::exit() {
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	presets.keyPressed(key);

	if (key == 'g') {
		presets.bGui = !presets.bGui;
	}
}

//--------------------------------------------------------------
void ofApp::drawScene()
{
	float _size1 = (size1) + 0.2;
	float _size2 = (size2);
	float _rotation = (rotation);
	float _rotationOffset = (rotationOffset);

	//--

	static ofColor colorBg = 32;
	ofClear(colorBg);

	ofPushStyle();
	ofPushMatrix();

	// some code from @Daandelange > https://github.com/Daandelange/ofxImGui/tree/master/example-sharedcontext

	ofParameter<ofFloatColor> background{ "Background", ofFloatColor::black };
	ofParameter<ofFloatColor> foreground{ "Foreground", ofFloatColor::black };
	ImVec4 color = { 0,0,0,1.0 };

	int a = ofMap(_size1, 0, 2, 255, 170);

	ofSetColor(color.x * 255, color.y * 255, color.z * 255, color.w * a);
	float _scale = 0.1f;
	float r = ofGetHeight()*_scale*_size1*(1);
	ofDrawCircle(ofGetWidth()*0.5f, ofGetHeight()*0.5f, r);

	float _scale2 = 0.2f;
	float staticAnimationPos = 1;
	int rectSize = _size2 + abs((((ofGetHeight() * _scale2 - _size2)*_size1))*(staticAnimationPos));
	//rectSize /= 2;

	ofTranslate(ofGetWidth()*.5f, ofGetHeight()*.5f);
	ofRotateDeg(ofGetElapsedTimef() * TWO_PI);

	float rot = _rotationOffset / 3.0;
	ofRotateDeg(ofGetElapsedTimef() * TWO_PI);
	ofRotateZDeg(_rotation * 45);
	ofScale(1.3);
	ofRotateDeg(rot);

	int a2 = ofMap(_size2, 0, size2.getMax(), 255, 225);

	ofSetColor(background->r * 255, background->g * 255, background->b * 255, background->a * a2);
	ofDrawRectangle(-rectSize * .5f, -rectSize * .5f, rectSize, rectSize);

	ofRotateDeg(rot);
	ofSetColor(background->r * 255, background->g * 255, background->b * 255, background->a * a2);
	ofDrawRectangle(-rectSize * .5f, -rectSize * .5f, rectSize, rectSize);

	ofRotateDeg(rot);
	ofSetColor(foreground->r * 255, foreground->g * 255, foreground->b * 255, foreground->a * a2);
	ofDrawRectangle(-rectSize * .5f, -rectSize * .5f, rectSize, rectSize);

	//// inner circle
	//ofFill();
	//ofSetColor(colorBg);
	//int r2 = ofMap(_size1, 0, size1.getMax(), 3, 20, true);
	//ofDrawCircle(0,0, r2);

	ofPopMatrix();
	ofPopStyle();
}

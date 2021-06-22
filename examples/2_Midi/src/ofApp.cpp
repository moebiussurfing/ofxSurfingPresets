#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetWindowPosition(-1920, 26);

	ofSetCircleResolution(200);

	// group
	params.setName("paramsGroup");
	params.add(size1.set("size1", 0.5, 0, 1));
	params.add(size2.set("size2", ofGetHeight() * 0.5, 0, ofGetHeight() * 0.25));
	params.add(rotation1.set("rotation1", 1, 0, 2));
	params.add(rotation2.set("rotation2", 180, 0, 360));

	presets.setImGuiAutodraw(true); // -> required true when using only one ImGui instance inside the add-ons of your ofApp
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
	float _size1 = (size1)+0.2;
	float _size2 = (size2);
	float _rotation1 = (rotation1);
	float _rotation2 = (rotation2);

	//--

	static ofColor colorBg = 255;
	//static ofColor colorBg = 32;
	ofClear(colorBg);

	ofPushStyle();
	ofPushMatrix();

	// some code from @Daandelange > https://github.com/Daandelange/ofxImGui/tree/master/example-sharedcontext

	ofParameter<ofFloatColor> color1{ "c1", ofFloatColor::black };
	ofParameter<ofFloatColor> color2{ "c2", ofFloatColor::black };
	ImVec4 color = { 0,0,0,1.0 };

	//int a1 = ofMap(_size1, 0, 2, 255, 170);
	int a2 = ofMap(_size2, 0, size2.getMax(), 200, 245);

	//-

	// extra shape
	//ofSetColor(color.x * 255, color.y * 255, color.z * 255, color.w * a1);
	//float _scale = 0.1f;
	//float r = ofGetHeight()*_scale*_size1*(1);
	////ofDrawCircle(ofGetWidth()*0.5f, ofGetHeight()*0.5f, r);
	//ofSetRectMode(OF_RECTMODE_CENTER);
	//ofDrawRectangle(ofGetWidth()*0.5f, ofGetHeight()*0.5f, 2*r, 2*r);
	//ofSetRectMode(OF_RECTMODE_CORNER);

	float _scale2 = _size1 / 5.f;
	//float _scale2 = 0.2f;
	int rectSize = _size2 + (ofGetHeight() * _scale2);
	//float staticAnimationPos = 1;
	//int rectSize = _size2 + abs((((ofGetHeight() * _scale2 - _size2)*_size1))*(staticAnimationPos));
	//rectSize /= 2;

	ofTranslate(ofGetWidth()*.5f, ofGetHeight()*.5f);

	//float yy = ofMap(_size2, 0, size2.getMax(), 0, - 20);
	float yy = 0;
	ofTranslate(0, - 2 * yy);

	ofRotateDeg(ofGetElapsedTimef() * TWO_PI);

	float rot = _rotation2 / 3.0;
	ofRotateDeg(ofGetElapsedTimef() * TWO_PI);
	ofRotateZDeg(_rotation1 * 45);
	ofScale(1.3);
	ofRotateDeg(rot);
	
	float ratio = ofMap(_rotation2, 0, 360, 1, 0.5f + _size1 * 0.4);
	float rangle = ofMap(_size1, 0, 1, 0, -30);

	ofSetColor(color1->r * 255, color1->g * 255, color1->b * 255, color1->a * a2);
	ofDrawRectangle(-rectSize * .5f, -rectSize * .5f, rectSize, rectSize);

	rectSize *= ratio;
	ofRotateDeg(rot);
	ofTranslate(0, yy);
	rot += rangle;

	ofSetColor(color1->r * 255, color1->g * 255, color1->b * 255, color1->a * a2);
	ofDrawRectangle(-rectSize * .5f, -rectSize * .5f, rectSize, rectSize);

	rectSize *= ratio;
	ofRotateDeg(rot);
	ofTranslate(0, yy);
	rot += rangle;

	ofSetColor(color2->r * 255, color2->g * 255, color2->b * 255, color2->a * a2);
	ofDrawRectangle(-rectSize * .5f, -rectSize * .5f, rectSize, rectSize);

	rectSize *= ratio;
	ofRotateDeg(rot);
	ofTranslate(0, yy);
	rot += rangle;

	ofSetColor(color2->r * 255, color2->g * 255, color2->b * 255, color2->a * a2);
	ofDrawRectangle(-rectSize * .5f, -rectSize * .5f, rectSize, rectSize);

	//// inner circle
	//ofFill();
	//ofSetColor(colorBg);
	//int r2 = ofMap(_size1, 0, size1.getMax(), 3, 20, true);
	//ofDrawCircle(0,0, r2);

	ofPopMatrix();
	ofPopStyle();
}

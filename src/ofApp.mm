#include "ofApp.h"

#import <Cocoa/Cocoa.h>
#include <sstream>

//--------------------------------------------------------------
void ofApp::setup(){
	
	ofSetDataPathRoot("../Resources/data/");
	
	camKeys[0] = '1'; camKeys[1] = '2'; camKeys[2] = '3';
	camKeys[3] = '4'; camKeys[4] = '5'; camKeys[5] = '6';
	camKeys[6] = '7'; camKeys[7] = '8'; camKeys[8] = '9';
	
	// setup window attributes
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofSetWindowTitle("Virtual Mapper");
	ofEnableSmoothing();
	
	// load
	settings.load("settings.xml");
	font.loadFont(FONT_NAME, 10);
	
	// ui
	setGUI();
	
	setWindowOnTop(isWindowOnTop);
	resetCam();
	
	// load model
	string path = settings.getValue("settings:screenPath", "");
	string name = settings.getValue("settings:screenName", "null");
	loadScreen(path, name);
	
	// load camera settings
	loadCams();
	camIndex = CAM_INDEX_DEFAULT;
	//updateCamList();
	
	// syphon setup
	syphonClient.setup();
	isSyphonChanged = true;
	syphonIndex = -1;
	syphonWidth = 0;
	syphonHeight = 0;
	
	syphonDir.setup();
	
	ofAddListener(syphonDir.events.serverAnnounced, this, &ofApp::syphonAnnounced);
	ofAddListener(syphonDir.events.serverRetired, this, &ofApp::syphonRetired);
}

//--------------------------------------------------------------
void ofApp::update(){
	
	isModalOpened = false;
	
	int sw = syphonClient.getWidth();
	int sh = syphonClient.getHeight();
	
	if (syphonWidth != sw || syphonHeight != sh) {
		
		scaleScreenUV(sw, sh);
		
		syphonWidth = sw;
		syphonHeight = sh;
	}
	
	if ( isSyphonChanged ) {
		
		if ( syphonIndex >= 0 ) {
			syphonClient.set(syphonDir.getDescription(syphonIndex));
			isSyphonChanged = false;
		}
	}
	
	// gui update
	camPos = grabCam.getPosition();
	ndCamX->setValue( camPos.x );
	ndCamY->setValue( camPos.y );
	ndCamZ->setValue( camPos.z );
	
	camEuler = grabCam.getOrientationEuler();
	msCamH->setValue( camEuler.x );
	msCamP->setValue( camEuler.y );
	msCamB->setValue( camEuler.z );
	
	// detect if cam moved
	if (camIndex != CAM_INDEX_DEFAULT) {
		
		if (cams[camIndex].position != grabCam.getPosition()
			|| (cams[camIndex].euler - grabCam.getOrientationEuler()).length() > 0.005f
			|| cams[camIndex].fov != grabCam.getFov()) {
			
			camIndex = CAM_INDEX_DEFAULT;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	grabCam.begin();

	ofBackground(0, 0, 0, 0);

	// bind
	ofEnableDepthTest();
	
	syphonClient.bind();
	
	// draw
	mesh.draw();
	
	// unbind
	syphonClient.unbind();
	
	if ( isShowWireframe ) {
		ofSetColor(255, 255, 255, 255);
		mesh.drawWireframe();
		
		ofDrawGrid(250.0f, 1.0f, false, true, true, true);
	}
	
	ofDisableDepthTest();
	
	grabCam.end();

	// cam list
	string camName;
	for (int i = CAM_INDEX_DEFAULT;i != cams.size(); i++) {
		
		if (i == CAM_INDEX_DEFAULT) {
			camName = CAM_DEFAULT_TEXT;
		} else {
			stringstream ss;
			ss << (i + 1);
			camName = "[" + ss.str() + "] " + cams[i].name;
		}
		
		if (i == camIndex) {
			ofSetColor(5, 140, 190);
		} else {
			ofSetColor(255, 255, 255);
		}
		
		font.drawString(camName, ofGetWidth() - 140, (i+2) * 20);
	}
	
	// instruction
	if (gui->isVisible()) {
		int x = ofGetWidth() - 230;
		int y = ofGetHeight() - 110;
		int row = 1;
		ofSetColor(128, 128, 128);
		font.drawString("drag(left mouse): orbit", x, y + row++ * 15 );
		font.drawString("drag(right mouse): zoom", x, y + row++ * 15 );
		font.drawString("hold [h] and drag: pan ", x, y + row++ * 15 );
		font.drawString("[h]: reset viewport", x, y + row++ * 15 );
		font.drawString("[1-9] or [up/down]: change camera", x, y + row++ * 15 );
		font.drawString("[q]: on/off HUD", x, y + row++ * 15 );
	}
	
	ofSetColor(255, 255, 255);
}

//--------------------------------------------------------------
void ofApp::exit() {
	
	gui->saveSettings("gui.xml");
	
	saveCams();
	
	settings.save("settings.xml");
}

//--------------------------------------------------------------
void ofApp::guiEvent(ofxUIEventArgs &e) {
	
	if (this == NULL) {
		return;
	}
	
	string name = e.widget->getName();
	
	if ( name == "3D LOAD" && !isModalOpened ) {
		
		isModalOpened = true;
		setWindowOnTop(false);
		ofFileDialogResult result = ofSystemLoadDialog();
		setWindowOnTop(isWindowOnTop);
		
		string path = result.getPath();
		string name = result.getName();
		loadScreen( path, name );
		
	} else if (name == "SYPHON LIST") {
		
		vector<int> indices = ddlSyphon->getSelectedIndeces();
		
		if (indices.size() > 0) {

			syphonIndex = indices[0];
			isSyphonChanged = true;
		}
		
	} else if ( name == "make window on top") {
		
		ofxUIToggle *t = (ofxUIToggle*) e.widget;
		setWindowOnTop(t->getValue());
		
	} else if ( name == "add cam" && !isModalOpened ) {
		
		isModalOpened = true;
		setWindowOnTop(false);
		string name = ofSystemTextBoxDialog("Camera Name");
		setWindowOnTop(isWindowOnTop);
		
		Camera nc;
		nc.name = name;
		nc.position.set( grabCam.getPosition() );
		nc.euler.set( grabCam.getOrientationEuler() );
		nc.fov = grabCam.getFov();
		
		cams.push_back(nc);
		camIndex = cams.size() - 1;
		
	} else if ( name == "remove cam") {
		
		if ( camIndex != CAM_INDEX_DEFAULT) {
			cams.erase( cams.begin() + camIndex );
			camIndex = CAM_INDEX_DEFAULT;
		}
	
	} else if ( name == "CAMERA FOV" ) {
		
		ofxUIMinimalSlider *s = (ofxUIMinimalSlider*) e.widget;
		grabCam.setFov( s->getValue() );
	
	} else if ( name == "CAMERA X" || name == "CAMERA Y" || name == "CAMERA Z" ) {

		grabCam.setPosition(ndCamX->getValue(),
						ndCamY->getValue(),
						ndCamZ->getValue());
	
	} else if ( name == "CAMERA H" || name == "CAMERA P" || name == "CAMERA B" ) {
		
		ofVec3f euler(msCamH->getValue(),
					  msCamP->getValue(),
					  msCamB->getValue());
		grabCam.setOrientation( euler );
	}
}

//--------------------------------------------------------------
void ofApp::syphonAnnounced(ofxSyphonServerDirectoryEventArgs &arg) {
	
	for (auto& dir: arg.servers) {
		
		ddlSyphon->addToggle(dir.appName.substr(0, DDL_MAX_LENGTH));
	}
	
	vector<ofxUILabelToggle*> toggles = ddlSyphon->getToggles();
	if ( toggles.size() > 0 ) {
		string firstToggleName = toggles[0]->getName().substr(0, DDL_MAX_LENGTH);
		ddlSyphon->activateToggle(firstToggleName);
		ddlSyphon->setLabelText(firstToggleName);
		syphonIndex = 0;
		isSyphonChanged = true;
	}
}

void ofApp::syphonRetired(ofxSyphonServerDirectoryEventArgs &arg)
{
	vector<ofxUILabelToggle*> toggles = ddlSyphon->getToggles();
	string name;
	
	for (auto& dir : arg.servers ) {
		
		name = dir.appName.substr(0, DDL_MAX_LENGTH);
		
		for (auto* t : toggles) {
			if ( name == t->getName() ) {
				ddlSyphon->removeToggle(name);
			}
		}
	}
	
	// activate index 0
	toggles = ddlSyphon->getToggles();
	if ( toggles.size() > 0 ) {
		string firstToggleName = toggles[0]->getName().substr(0, DDL_MAX_LENGTH);
		ddlSyphon->activateToggle(firstToggleName);
		ddlSyphon->setLabelText(firstToggleName);
		syphonIndex = 0;
		isSyphonChanged = true;
	}
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

	if (key == 'r') {
		
		resetCam();
	
	} else if (key == 'q') {
		
		gui->toggleVisible();
		
	} else if (key == OF_KEY_UP) {
		
		if (--camIndex < 0)
			camIndex = cams.size() - 1;
		
		changeCam(camIndex);
		
	} else if (key == OF_KEY_DOWN) {
	
		if (++camIndex >= cams.size())
			camIndex = 0;
		
		changeCam(camIndex);
		
	} else {
		
		 for (int i = 0; i < CAM_KEYS_SIZE; i++) {
			
			if (key == camKeys[i])
				changeCam(i);
		 }
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

	ofxUIRectangle *rect = gui->getRect();
	
	if ( rect->inside(x, y) ) {
		
		grabCam.setMouseActions( false );
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

	grabCam.setMouseActions( true );
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

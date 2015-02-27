#include "ofApp.h"

#include <sstream>

//--------------------------------------------------------------
void ofApp::setup(){

#ifdef TARGET_OSX
	ofSetDataPathRoot("../Resources/data/");
#endif	

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
    defaultTex.loadImage("default_texture.png");
	
	// ui
	setGUI();
    // view
    platformWindow.setWindowOnTop(isWindowOnTop);
    resetCam();
	
	// load model
	string path = settings.getValue("settings:screenPath", "");
	string name = settings.getValue("settings:screenName", "null");
	loadScreen(path, name);
	
	// load camera settings
	loadCams();
	camIndex = CAM_INDEX_DEFAULT;
	//updateCamList();
	
    // source
	texWidth = 0;
	texHeight = 0;
    receiver.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
	
	isModalOpened = false;
    
    // update input
    receiver.update();
	
    int w, h;
    
    if ( receiver.isEnabled() ) {
        w = receiver.getWidth();
        h = receiver.getHeight();
    } else {
        w = defaultTex.width;
        h = defaultTex.height;
    }
	
	if (texWidth != w || texHeight != h) {
		
		scaleScreenUV(w, h);
		
		texWidth = w;
		texHeight = h;
	}
	
#ifdef TARGET_OSX
	if ( receiver.isChanged() ) {
        
        // update input list
        vector<string> inputs = receiver.getInputs();
        
        ddlInput->clearToggles();
        for (string i : inputs) {
            ddlInput->addToggle( i );
        }

        ddlInput->addToggles( receiver.getInputs() );
        ddlInput->setLabelText( (receiver.getActiveInput()).substr(0, DDL_MAX_LENGTH) );
	}
#endif
	
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
    
    receiver.next();
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	grabCam.begin();

	ofBackground(0, 0, 0, 0);

	ofEnableDepthTest();
	
    // draw screen mesh
    if ( receiver.isEnabled() ) {
        receiver.bind();
    } else {
        defaultTex.bind();
    }
	screen.draw();
    if ( receiver.isEnabled() ) {
        receiver.unbind();
    } else {
        defaultTex.unbind();
    }
    
    // draw stage mesh
    ofSetColor(16);
    stage.draw();
    ofSetColor(128);
    stage.drawWireframe();
    
    // optional wireframe
	if ( isShowWireframe ) {
		ofSetColor(255, 255, 255, 255);
		screen.drawWireframe();
	}
    if ( isShowGrid ) {
        ofDrawGrid(2000.0f, 8.0f, false, true, true, true);
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
		font.drawString("[r]: reset viewport", x, y + row++ * 15 );
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

	receiver.exit();
}

//--------------------------------------------------------------
void ofApp::guiEvent(ofxUIEventArgs &e) {
	
	if (this == NULL) {
		return;
	}
	
	string name = e.widget->getName();
	
	if ( name == "3D LOAD" && !isModalOpened ) {
		
		isModalOpened = true;
		platformWindow.setWindowOnTop(false);
		ofFileDialogResult result = ofSystemLoadDialog("Load Model..");
        platformWindow.setWindowOnTop(isWindowOnTop);
		
		string path = result.getPath();
		string name = result.getName();
        
        // check if its extension is '.obj'
        cout << path << endl;
        cout << name << endl;
        
        string ext = name.substr(name.find_last_of(".") + 1);
        if ( ext == "obj" || ext == "dae" ) {
            loadScreen( path, name );
        } else {
            alert("This file type is not supported.");
        }
		
	}
#ifdef TARGET_OSX
	else if (name == "INPUT LIST") {
		
		vector<int> indices = ddlInput->getSelectedIndeces();
		
		if (indices.size() > 0) {
            
            receiver.setInput( indices[0] );
		}
		
	}
#endif
	else if ( name == "make window on top") {
		
		ofxUIToggle *t = (ofxUIToggle*) e.widget;
		platformWindow.setWindowOnTop(t->getValue());
		
	} else if ( name == "add cam" && !isModalOpened ) {
		
		isModalOpened = true;
		platformWindow.setWindowOnTop(false);
		string name = ofSystemTextBoxDialog("Camera Name");
		platformWindow.setWindowOnTop(isWindowOnTop);
		
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

/*
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
    cout << "SyphonRetired" << endl;
    
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
	
	// if other sources exists, activate index 0
    // otherwise, set -1 (show default texture)
    cout << syphonDir.size() << endl;
    
	if ( syphonDir.size() > 0 ) {
        toggles = ddlSyphon->getToggles();
		string firstToggleName = toggles[0]->getName().substr(0, DDL_MAX_LENGTH);
		ddlSyphon->activateToggle(firstToggleName);
		ddlSyphon->setLabelText(firstToggleName);
		syphonIndex = 0;
    } else {
        ddlSyphon->clearToggles();
        ddlSyphon->setLabelText("");
        syphonIndex = -1;
    }
    
    isSyphonChanged = true;
}
 */


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

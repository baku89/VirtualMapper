#include "ofApp.h"


#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

//--------------------------------------------------------------
void ofApp::setup(){

	
//#ifdef TARGET_OSX
//	ofSetDataPathRoot("../Resources/data/");
//#endif

//	camKeys[0] = '1'; camKeys[1] = '2'; camKeys[2] = '3';
//	camKeys[3] = '4'; camKeys[4] = '5'; camKeys[5] = '6';
//	camKeys[6] = '7'; camKeys[7] = '8'; camKeys[8] = '9';

	// setup window attributes
	ofEnableNormalizedTexCoords();
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofSetWindowTitle("Virtual Mapper");
	ofEnableSmoothing();

	
	ofLoadImage(defaultTex, "default_screen.jpg");
	
	gui.setup();
	ImGui::GetIO().MouseDrawCursor = false;
	
	sharedInput.setup();
	
	ofxXmlSettings settings;
	
	settings.load("settings.xml");
	
	scene.open(settings.getValue("scene:path", "default_scene.abc"));
	
	
	
	
	// load
//	settings.load("settings.xml");
//    defaultTex.load("default_texture.jpg");
	
	// ui
//	setGUI();
    // view
//    platformWindow.setWindowOnTop(isWindowOnTop);
//    resetCam();
	
	// load model
//	string path = settings.getValue("settings:screenPath", "");
//	string name = settings.getValue("settings:screenName", "null");
//	loadScreen(path, name);
	
	// load camera settings
//	loadCams();
//	camIndex = CAM_INDEX_DEFAULT;
	//updateCamList();
	
    // source
//	texWidth = 0;
//	texHeight = 0;
//    receiver.setup();

}

//--------------------------------------------------------------
void ofApp::update(){
//	
//	isModalOpened = false;
	
    // update input
//    receiver.update();
	
//    int w, h;
//	
//	w = defaultTex.getWidth();
//	h = defaultTex.getHeight();
//	
//	if (texWidth != w || texHeight != h) {
//		texWidth = w;
//		texHeight = h;
//		scaleScreenUV();
//	}
	

//	if ( receiver.isChanged() ) {
	
        // update input list
//        vector<string> inputs = receiver.getInputs();
	
		/*
        ddlInput->clearToggles();
        for (string i : inputs) {
            ddlInput->addToggle( i );
        }
		*/
		
		/*
        ddlInput->addToggles( receiver.getInputs() );
        ddlInput->setLabelText( (receiver.getActiveInput()).substr(0, DDL_MAX_LENGTH) );
	}
            ddlInput->addToggle( i.substr(0, DDL_MAX_LENGTH) );
        }
        ddlInput->setLabelText( (receiver.getActiveInput()).substr(0, DDL_MAX_LENGTH) );
		*/
//	}
	
	// gui update
	/*
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
	 */
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	
	ofBackground(0);
	
	grabCam.begin();
	{
		ofEnableDepthTest();
		
		if (inputSource == INPUT_SYPHON) {
			sharedInput.bind();
		} else {
			defaultTex.bind();
		}
		
		scene.draw();
		
		if (inputSource == INPUT_SYPHON) {
			defaultTex.unbind();
		} else {
			defaultTex.unbind();
		}
		
		// draw screen mesh
//		defaultTex.bind();
//		screen.draw();
//		defaultTex.unbind();
		
		// draw scene mesh
		/*
		ofSetColor(16);
		scene.draw();
		ofSetColor(128);
		scene.drawWireframe();
		*/
		
		/*
		// optional wireframe
		if ( isShowWireframe ) {
			ofSetColor(255, 255, 255, 255);
			screen.drawWireframe();
		}
		if ( isShowGrid ) {
			ofDrawGrid(2000.0f, 8.0f, false, true, true, true);
		}
		 */
		ofDisableDepthTest();
	}
	grabCam.end();

	// cam list
	/*
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
	*/
	
	// instruction
	/*
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
	*/
	
	
	
	gui.begin();
	{
		if (ImGui::Button("Open Scene")) {
			scene.openWithDialog();
		}
		
		
		if (ImGui::CollapsingHeader("Camera", true)) {
			
			
			
			for (int i = 0; i < scene.getNumCameras(); i++) {
				if (ImGui::RadioButton(scene.getCameraName(i).c_str(), &camIndex, i)) {
					
					ofCamera cam = scene.getCamera(camIndex);
					
					grabCam.setGlobalPosition(cam.getGlobalPosition());
					grabCam.setOrientation(cam.getOrientationQuat());
					grabCam.setFov(cam.getFov());
				}
			}
			
			// matrix
			if (ImGui::TreeNode("Transform")) {
				
				
				float *pos = grabCam.getGlobalPosition().getPtr();
				ImGui::InputFloat3("Position", pos);
				grabCam.setGlobalPosition(pos[0], pos[1], pos[2]);
				
				float *euler = grabCam.getOrientationEuler().getPtr();
				ImGui::InputFloat3("Orientation", euler);
				ofVec3f ne(euler[0], euler[1], euler[2]);
				grabCam.setOrientation(ne);
				
				float fov = grabCam.getFov();
				ImGui::SliderFloat("Fov", &fov, 0, 180);
				grabCam.setFov(fov);
				
				ImGui::TreePop();
			}
		}
		
		if (ImGui::CollapsingHeader("Input Source", true)) {
			
			
			ImGui::RadioButton("Image", &inputSource, INPUT_IMAGE);
			ImGui::SameLine();
			
			ImGui::RadioButton("Syphon", &inputSource, INPUT_SYPHON);
			
			if (inputSource == INPUT_SYPHON) {
				
				
				string itemsStr = "";
				
				for (auto& n : sharedInput.getNames()) {
					itemsStr += n + '\0';
				}
			
				const char* items = itemsStr.c_str();
				static int index = -1;
				
				if (ImGui::Combo("Server", &index, items)) {
					ofLogNotice() << index;
					sharedInput.setIndex(index);
				}
			}
			
		}
	}
	gui.end();
}

//--------------------------------------------------------------
void ofApp::exit() {
	
	scene.exit();
	
	
	ofxXmlSettings settings;
	
	settings.setValue("scene:path", scene.getPath());
	
	settings.saveFile("settings.xml");
	
//	gui->saveSettings("gui.xml");
	
//	saveCams();
	
//	settings.save("settings.xml");

//	receiver.exit();
}

//--------------------------------------------------------------
/*
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
 
	} else if (name == "INPUT LIST") {
		
		vector<int> indices = ddlInput->getSelectedIndeces();
		
		if (indices.size() > 0) {
            
            receiver.setInput( indices[0] );
		}
 
	} else if ( name == "flip H" || name == "flip V" ) {
		
		cout << "change uv flip settings" << endl;
		scaleScreenUV();

	} else if ( name == "make window on top") {
		
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
*/

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
void ofApp::keyReleased(int key){
	
	/*
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
	 */
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

	bool guiCaptured = ImGui::GetIO().WantCaptureMouse;
	grabCam.setMouseActionsEnabled(!guiCaptured);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {


}

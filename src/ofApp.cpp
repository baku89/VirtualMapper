#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	
//	camKeys[0] = '1'; camKeys[1] = '2'; camKeys[2] = '3';
//	camKeys[3] = '4'; camKeys[4] = '5'; camKeys[5] = '6';
//	camKeys[6] = '7'; camKeys[7] = '8'; camKeys[8] = '9';

	// setup window attributes
	ofEnableNormalizedTexCoords();
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofSetWindowTitle("Virtual Mapper");
	ofEnableSmoothing();
	
	// setup imgui
	ImGuiIO * io = &ImGui::GetIO();
	ImFontConfig font_config;
	font_config.OversampleH = 1;
	font_config.OversampleV = 1;
	io->Fonts->AddFontFromFileTTF(&ofToDataPath("Karla-Regular.ttf")[0], 14.f, &font_config);
	
	gui.setup();
	
	ImGuiStyle *style = &ImGui::GetStyle();
	style->WindowRounding = 0;
	style->ScrollbarSize = 3;
	style->ScrollbarRounding = 0;
	style->FramePadding = ImVec2(6, 4);
	style->ItemSpacing = ImVec2(4, 8);
	style->FrameRounding = 2;
	style->GrabRounding = 2;
	
	
	// setup source
	sourceManager.setup();
	
	// load settings
	ofxXmlSettings settings("settings.xml");
	
	showControl = settings.getValue("showControl", true);
	
	scene.loadSettings(settings);
	sourceManager.loadSettings(settings);
	
	// load camera settings
//	loadCams();
//	camIndex = CAM_INDEX_DEFAULT;
	//updateCamList();

}

//--------------------------------------------------------------
void ofApp::update(){
//	
//	isModalOpened = false;
	
    // update input
//    receiver.update();
	
//    int w, h;
//
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
		
		sourceManager.bind();
		
		scene.draw();
		
		sourceManager.unbind();
		
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
	
	
	
	drawImGui();
}

//--------------------------------------------------------------
void ofApp::drawImGui() {
	
	gui.begin();
	
	if (showControl) {
		
		static bool p_open = true;
		
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::Begin("Control", &p_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
		ImGui::SetWindowSize(ImVec2(ImGui::GetWindowWidth(), ofGetHeight()));
		{
			if (ImGui::Button("Hide Control")) {
				showControl = false;
			}
			
			if (ImGui::CollapsingHeader("Scene")) {
				
				scene.drawImGui();
			}
			
			/*
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
			 */
			
			if (ImGui::CollapsingHeader("Source", true)) {
				
				sourceManager.drawImGui();
				
			}
		}
		ImGui::End();
		
	} else {
		
		static bool p_open = true;
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(100, 40));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
		ImGui::Begin("Show Control", &p_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
		{
			if (ImGui::Button("Show Control")) {
				showControl = true;
			}
		}
		ImGui::End();
		ImGui::PopStyleColor();
	}
	
	gui.end();
}

//--------------------------------------------------------------
void ofApp::exit() {
	
	scene.exit();
	
	
	ofxXmlSettings settings;
	
	settings.setValue("showControl", showControl);
	
	scene.saveSettings(settings);
	sourceManager.saveSettings(settings);
	
	settings.saveFile("settings.xml");
	
//	gui->saveSettings("gui.xml");
	
//	saveCams();
	
//	settings.save("settings.xml");

//	receiver.exit();
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	
	if (key == 'c') {
		showControl = !showControl;
	}
	
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

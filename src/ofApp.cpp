#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){

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
	viewManager.setup();
	sourceManager.setup();
	
	// load settings
	ofxXmlSettings settings("settings.xml");
	
	showControl = settings.getValue("showControl", true);
	
	sceneManager.loadSettings(settings);
	viewManager.loadSettings(settings);
	sourceManager.loadSettings(settings);
}

//--------------------------------------------------------------
void ofApp::update(){

	sceneManager.update();
	viewManager.update(sceneManager);
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	
	ofBackground(0);
	
	
	viewManager.draw(sceneManager, sourceManager);

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
			
			sceneManager.drawImGui();
			viewManager.drawImGui(sceneManager);
			sourceManager.drawImGui();
			
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
	
	// save settings
	ofxXmlSettings settings;
	
	settings.setValue("showControl", showControl);
	
	sceneManager.saveSettings(settings);
	viewManager.saveSettings(settings);
	sourceManager.saveSettings(settings);
	
	settings.saveFile("settings.xml");
	
//	saveCams();
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
	viewManager.setMouseActionsEnabled(!guiCaptured);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {


}

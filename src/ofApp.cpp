#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
#ifdef RELEASE
	ofSetDataPathRoot("../Resources/data/");
#endif

	// setup window attributes
	ofEnableNormalizedTexCoords();
	ofSetFrameRate(60);
	ofSetWindowTitle("Virtual Mapper");
	ofEnableSmoothing();
	
	// setup imgui
	ImOf::SetFont();
	gui.setup();
	ImOf::SetStyle();
	
	
	// setup source
	managers.push_back(&sceneManager);
	managers.push_back(&viewManager);
	managers.push_back(&sourceManager);
	managers.push_back(&miscManager);
	
	for (auto& manager : managers) {
		manager->setup();
	}
	
	// event
	ofAddListener(sceneManager.cameraListUpdated, this, &ofApp::cameraListUpdated);
	
	
	// load settings
	ofxXmlSettings settings("settings.xml");
	
	showControl = settings.getValue("showControl", true);
	
	for (auto& manager : managers) {
		manager->loadSettings(settings);
	}
	
}

//--------------------------------------------------------------
void ofApp::update(){
	
	for (auto& manager : managers) {
		manager->update();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);
	
	viewManager.draw(sceneManager, sourceManager);
	
	drawImGui();
}

//--------------------------------------------------------------
void ofApp::drawImGui() {
	
	gui.begin();
	
	if (showControl) {
		
		static bool p_open = true;
		
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSizeConstraints(ImVec2(250, ofGetHeight()), ImVec2(ofGetWidth(), ofGetHeight()));
		ImGui::Begin("Control", &p_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
		ImGui::SetWindowSize(ImVec2(ImGui::GetWindowWidth(), ofGetHeight()));
		{
			if (ImGui::Button("Hide Control")) {
				showControl = false;
			}
			
			for (auto& manager : managers) {
				manager->drawImGui();
			}
			
		}
		ImGui::End();
		
	} else {
		
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(100, 40));
		ImOf::BeginTransparentWindow();
		{
			if (ImGui::Button("Show Control")) {
				showControl = true;
			}
		}
		ImOf::EndTransparentWindow();
	}
	
	gui.end();
}

//--------------------------------------------------------------
void ofApp::exit() {
	
	// save settings
	ofxXmlSettings settings;
	
	settings.setValue("showControl", showControl);
	
	for (auto& manager : managers) {
		manager->saveSettings(settings);
	}
	
	settings.saveFile("settings.xml");
}

//--------------------------------------------------------------
void ofApp::cameraListUpdated(vector<CameraInfo> &cameraList) {
	viewManager.updateCameraList(cameraList);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	
	if (key == 'c') {
		showControl = !showControl;
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {


}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
	
	for (auto& manager : managers) {
		manager->dragEvent(dragInfo);
	}
	
}

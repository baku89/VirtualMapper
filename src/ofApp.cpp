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
	ImOf::SetFont();
	gui.setup();
	ImOf::SetStyle();
	
	
	// setup source
	viewManager.setup(&sceneManager);
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
	viewManager.update();
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
		ImGui::Begin("Control", &p_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
		ImGui::SetWindowSize(ImVec2(ImGui::GetWindowWidth(), ofGetHeight()));
		{
			if (ImGui::Button("Hide Control")) {
				showControl = false;
			}
			
			sceneManager.drawImGui();
			viewManager.drawImGui();
			sourceManager.drawImGui();
			
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
	
	viewManager.drawImPopup();
	
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

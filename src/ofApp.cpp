#include "ofApp.h"


#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

//--------------------------------------------------------------
void ofApp::setup(){
	
	// setup window attributes
	ofEnableNormalizedTexCoords();
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofSetWindowTitle("Virtual Mapper");
	ofEnableSmoothing();

	
	
	gui.setup();
	ImGui::GetIO().MouseDrawCursor = false;
	
	externalInput.setup();
	
	ofxXmlSettings settings;
	
	settings.load("settings.xml");
	
	// scene
	scene.open(settings.getValue("scene:path", "default_scene.abc"));
	
	// input source
	inputSource = settings.getValue("options:inputSource", INPUT_IMAGE);
	imageInput.load(settings.getValue("image:path", DEFAULT_IMAGE_PATH));
	
	// options
	isWindowTop		= settings.getValue("options:isWindowTop", false);
	showGrid		= settings.getValue("options:showGrid", true);
	gridSize		= settings.getValue("options:gridSize", 100.0f);
	showWireframe	= settings.getValue("options:showWireframe", false);
	
	grabCam.setFixUpDirectionEnabled(true);
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	
	ofBackground(0);
	
	grabCam.begin();
	{
		ofEnableDepthTest();
		
		if (inputSource == INPUT_IMAGE) {
			imageInput.bind();
		} else if (inputSource == INPUT_SYPHON) {
			externalInput.bind();
		}
		
		scene.draw();
		
		if (inputSource == INPUT_IMAGE) {
			imageInput.unbind();
		} else if (inputSource == INPUT_SYPHON) {
			externalInput.unbind();
		}
		
		if (showGrid) {
			ofDrawGrid(gridSize / 20.0f, 20.0f);
		}
		
		if (showWireframe) {
			scene.drawWireframe();
		}
		
		ofDisableDepthTest();
	}
	grabCam.end();
	
	drawGui();
}

//--------------------------------------------------------------
void ofApp::drawGui() {
	
	gui.begin();
	
	if (ImGui::Button("Open Scene")) {
		scene.openWithDialog();
	}
	
	
	if (scene.isLoaded()) {
		ImGui::Text("%s", scene.getPath().c_str());
	} else {
		ImGui::Text("-");
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
			ImGui::InputFloat3("Pos", pos);
			grabCam.setGlobalPosition(pos[0], pos[1], pos[2]);
			
			float *euler = grabCam.getOrientationEuler().getPtr();
			ImGui::InputFloat3("Rot", euler);
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
		
		if (inputSource == INPUT_IMAGE) {
			
			if (ImGui::Button("Open Image")) {
				ofFileDialogResult result = ofSystemLoadDialog("Load Image File (.abc)", false, ofToDataPath("."));
				if (result.bSuccess) {
					imageInput.load(result.getPath());
				} else {
					imageInput.load(DEFAULT_IMAGE_PATH);
				}
			}
			
			if (imageInput.isLoaded()) {
				ImGui::Text("%s", imageInput.getPath().c_str());
			} else {
				ImGui::Text("No Image");
			}
			
			
		} else if (inputSource == INPUT_SYPHON) {
			
			const vector<string> names = externalInput.getNames();
			
			string itemsStr = "";
			
			for (auto& n : names) {
				itemsStr += n + "\0";
			}
			
			const char* items = itemsStr.c_str();
			static int index = -1;
			
			if (ImGui::Combo("Server", &index, items)) {
				ofLogNotice() << index;
				externalInput.setIndex(index);
			}
		}
	}
	
	if (ImGui::CollapsingHeader("Options")) {
		
		if (ImGui::Checkbox("Always Window on Top", &isWindowTop)) {
			ofxTopWindow::setWindowOnTop(isWindowTop);
		}
		
		ImGui::Checkbox("Show Grid", &showGrid);
		if (showGrid) {
			ImGui::SliderFloat("Grid Size", &gridSize, 10, 100);
		}
		
		ImGui::Checkbox("Show Wireframe", &showWireframe);
	}
	
	gui.end();
}

//--------------------------------------------------------------
void ofApp::exit() {
	
	scene.exit();

	ofxXmlSettings settings;
	
	settings.setValue("image:path", imageInput.getPath());
	settings.setValue("scene:path", scene.getPath());
	
	settings.setValue("options:inputSource", inputSource);
	
	settings.setValue("options:isWindowTop", isWindowTop);
	settings.setValue("options:showGrid", showGrid);
	settings.setValue("options:gridSize", gridSize);
	settings.setValue("options:showWireframe", showWireframe);
	
	settings.saveFile("settings.xml");
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

	bool guiCaptured = ImGui::GetIO().WantCaptureMouse;
	grabCam.setMouseActionsEnabled(!guiCaptured);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {


}

#pragma once

#include "ofxImGui.h"
#include "ofxXmlSettings.h"
#include "ofxGrabCam.h"
#include "ofxTopWindow.h"

#include "ImOf.h"
#include "CameraInfo.h"
#include "SourceManager.h"
#include "SceneManager.h"

static void setMatrix4x4(ofxXmlSettings &settings, const string &tag, ofMatrix4x4 m) {
	
	settings.addTag(tag);
	settings.pushTag(tag);
	{
		float *mp = m.getPtr();
		for (int j = 0; j < 4; j++) {
			for (int i = 0; i < 4; i++) {
				settings.addValue("m" + ofToString(j) + ofToString(i), mp[j*4 + i]);
			}
		}
	}
	settings.popTag();
}

static ofMatrix4x4 getMatrix4x4(ofxXmlSettings &settings, const string &tag) {
	
	ofMatrix4x4 m;
	
	if (settings.tagExists(tag)) {
		settings.pushTag(tag);
		
		float values[16];
		for (int j = 0; j < 4; j++) {
			for (int i = 0; i < 4; i++) {
				values[j*4 + i] = settings.getValue("m" + ofToString(j) + ofToString(i), (double)(i==j ? 1 : 0));
			}
		}
		
		m.set(values);
		settings.popTag();
	}
	return m;
}


class ViewManager {
public:
	
	ViewManager() : cameraIndex(0) {}
	
	void setup() {
		cameraList  = new vector<CameraInfo>();
		
		resetCamera();
		
		ofAddListener(ofEvents().mousePressed, this, &ViewManager::mousePressed);
		ofAddListener(ofEvents().keyPressed, this, &ViewManager::keyPressed);
	}
	
	void drawImGui() {
		
		if (ImGui::CollapsingHeader("View")) {
			
			if (ImGui::TreeNode("Cameras")) {
				string cameraNames = "(Custom)";
				cameraNames += '\0';
				
				int i = 1;
				
				for (auto& camera : *cameraList) {
					cameraNames += ofToString(i++) + ": " + camera.name + '\0';
				}
				
				if (ImGui::Combo("", &cameraIndex, cameraNames.c_str())) {
					
					if (cameraIndex >= 1) {
						applyCurrentCameraInfo();
					}
				}
				
				// Coordinates
				ImGui::Text("Coord");
					
				float *pos = grabCam.getGlobalPosition().getPtr();
				ImGui::InputFloat3("Pos", pos);
				grabCam.setGlobalPosition(pos[0], pos[1], pos[2]);
			
				
				float *euler = grabCam.getOrientationEuler().getPtr();
				ImGui::InputFloat3("Rot", euler);
				const ofVec3f ne(euler[0], euler[1], euler[2]);
				grabCam.setOrientation(ne);
				
				float fov = grabCam.getFov();
				ImGui::SliderFloat("Fov", &fov, 0, 180);
				grabCam.setFov(fov);
				
				ImGui::TreePop();
			}
			
			if (ImGui::TreeNode("Rendering")) {
			
				ImGui::Checkbox("Show Wireframe", &showWireframe);
				ImGui::Checkbox("Show Grid", &showGrid);
				
				if (ImGui::Checkbox("Show Window On Top", &showWindowTop)) {
					ofxTopWindow::setWindowOnTop(showWindowTop);
				}
				
				ImGui::TreePop();
			}
		}

	}
	
	void drawImPopup() {
		
		if (cameraIndex >= 1) {
		
			ImOf::BeginPopup();
			ImGui::Text("%d: %s", cameraIndex, (*cameraList)[cameraIndex - 1].name.c_str());
			ImGui::SetWindowPos(ImVec2(ofGetWidth() - ImGui::GetWindowWidth() - 10 , 10));
			ImOf::EndPopup();
		}
	}
	
	void loadSettings(ofxXmlSettings &settings) {
		settings.pushTag("view");
		
		showWireframe = settings.getValue("showWireframe", false);
		showGrid = settings.getValue("showGrid", showGrid);
		showWindowTop = settings.getValue("showWindowTop", false);
		ofxTopWindow::setWindowOnTop(showWindowTop);
		
		settings.pushTag("camera");
		{
			cameraIndex = settings.getValue("index", 0);
			
			if (cameraIndex == 0) {
				
				if (settings.tagExists("info")) {
					settings.pushTag("info");
					
					grabCam.setTransformMatrix( getMatrix4x4(settings, "matrix") );
					grabCam.setFov( settings.getValue("fov", 40) );
					grabCam.setFixUpDirectionEnabled( settings.getValue("fixUpDirection", true) );
					
					settings.popTag();
				}
			
			} else if (cameraIndex <= cameraList->size()) {
				applyCurrentCameraInfo();
			}
		}
		settings.popTag();
		
		settings.popTag();
	}
	
	void saveSettings(ofxXmlSettings &settings) {
		
		settings.addTag("view");
		settings.pushTag("view");
		
		settings.setValue("showWireframe", showWireframe);
		settings.setValue("showGrid", showGrid);
		
		settings.addTag("camera");
		settings.pushTag("camera");
		{
			settings.setValue("index", cameraIndex);
			
			if (cameraIndex == 0) {
				settings.addTag("info");
				settings.pushTag("info");
				
				setMatrix4x4(settings, "matrix", grabCam.getGlobalTransformMatrix());
				settings.setValue("fov", grabCam.getFov());
				settings.setValue("fixUpDirection", grabCam.getFixUpDirectionEnabled());
				
				settings.popTag();
			}
		}
		settings.popTag();
		
		settings.popTag();
	}
	
	void updateCameraList(vector<CameraInfo> &_cameraList) {
		cameraList = &_cameraList;
	}
	
	void update() {
		
		if (cameraIndex >= 1) {
			
			CameraInfo camInfo = (*cameraList)[cameraIndex - 1];
			ofMatrix4x4 mg = grabCam.getGlobalTransformMatrix();
			
			if ( camInfo.fov != grabCam.getFov() || !equalMatrix(camInfo.matrix, mg) ) {
				cameraIndex = 0;
			}
		}
	}
	
	
	void draw(SceneManager &sceneManager, SourceManager &sourceManager) {
		
		grabCam.begin();
		{
			ofSetColor(255);
			
			ofEnableDepthTest();
			
			sourceManager.bind();
			
			sceneManager.draw();
			
			sourceManager.unbind();
			
			if (showWireframe) {
				sceneManager.drawWireframe();
			}
			
			if (showGrid) {
				ofPushStyle();
				{
					
					ofSetLineWidth(2);
					ofDrawAxis(400);
					
					ofSetLineWidth(1);
					ofSetColor(128);
					ofPushMatrix();
					{
						ofRotateZ(90);
						ofDrawGridPlane(100.0f, 10);
					}
					ofPopMatrix();
				
				}
				ofPopStyle();
			}
			
			ofDisableDepthTest();
		}
		grabCam.end();
		
	}
	
private:
	
	bool equalMatrix(ofMatrix4x4 &a, ofMatrix4x4 &b) {
		
		const float *ap = a.getPtr();
		const float *bp = b.getPtr();
		
		for (int i = 0; i < 16; i++) {
			if (abs(ap[i] - bp[i]) > 0.001) {
				return false;
			}
		}
		
		return true;
	}
	
	void applyCurrentCameraInfo() {
		if (cameraIndex == 0) {
			resetCamera();
		}
		
		CameraInfo camInfo = (*cameraList)[cameraIndex - 1];
		
		grabCam.setTransformMatrix(camInfo.matrix);
		grabCam.setFov(camInfo.fov);
		grabCam.setFixUpDirectionEnabled(false);
	}
	
	void resetCamera() {
		grabCam.setFixUpDirectionEnabled(true);
		grabCam.resetTransform();
		grabCam.setPosition(300, 100, 300);
		grabCam.lookAt(ofVec3f(0, 0, 0), ofVec3f(0, 1, 0));
		grabCam.setFov(40);
		grabCam.setNearClip(1);
		grabCam.setFarClip(10000);
	}
	
	// events
	void mousePressed(ofMouseEventArgs & args) {
		
		bool guiCaptured = ImGui::GetIO().WantCaptureMouse;
		grabCam.setMouseActionsEnabled(!guiCaptured);
	}
	
	void keyPressed(ofKeyEventArgs & args) {
		
		int ci = args.key - '0';
		if (1 <= ci &&  ci <= cameraList->size())  {
			cameraIndex = ci;
			applyCurrentCameraInfo();

		} else if (args.key == 'r') {
			resetCamera();
		}
	}

	
	// members
	
	ofxGrabCam			grabCam;
	vector<CameraInfo>	*cameraList;
	
	int					cameraIndex;
	bool				showWireframe, showGrid, showWindowTop;
	
	
};

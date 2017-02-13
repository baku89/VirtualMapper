#pragma once

#include "ofxImGui.h"
#include "ofxXmlSettings.h"
#include "ofxGrabCam.h"

#include "SourceManager.h"
#include "SceneManager.h"



class ViewManager {
public:
	
	ViewManager() : cameraIndex(0) {}
	
	void setup() {
		grabCam.setFixUpDirectionEnabled(true);
		grabCam.setPosition(100, 36, 100);
		grabCam.lookAt(ofVec3f(0, 0, 0), ofVec3f(0, 1, 0));
		grabCam.setNearClip(1);
		grabCam.setFarClip(10000);
	}
	
	void drawImGui(SceneManager &sceneManager) {
		
		if (ImGui::CollapsingHeader("View")) {
			
			if (ImGui::TreeNode("Cameras")) {
				string cameraNames = "(Custom)";
				cameraNames += '\0';
				
				int i = 0;
				
				for (auto& camera : sceneManager.getCameras()) {
					cameraNames += "[" + ofToString(i++) + "] - " + camera.name + '\0';
				}
				
				
				if (ImGui::Combo("", &cameraIndex, cameraNames.c_str())) {
					
					if (cameraIndex >= 1) {
						CameraInfo camInfo = sceneManager.getCameras()[cameraIndex - 1];
						
						grabCam.setTransformMatrix(camInfo.matrix);
						grabCam.setFov(camInfo.fov);
						//grabCam.setNearClip(camInfo.nearClip);
						//grabCam.setFarClip(camInfo.farClip);
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
				
				ImGui::TreePop();
			}
		}

	}
	
	void loadSettings(ofxXmlSettings &settings) {
		settings.pushTag("view");
		
		showWireframe = settings.getValue("showWireframe", false);
		showGrid = settings.getValue("showGrid", showGrid);
		
		settings.popTag();
	}
	
	void saveSettings(ofxXmlSettings &settings) {
		
		settings.addTag("view");
		settings.pushTag("view");
		
		settings.setValue("showWireframe", showWireframe);
		settings.setValue("showGrid", showGrid);
		
		settings.popTag();
	}
	
	void update(SceneManager &sceneManager) {
		
		if (cameraIndex >= 1) {
			
			CameraInfo camInfo = sceneManager.getCameras()[cameraIndex - 1];
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
				ofSetColor(128);
				sceneManager.drawWireframe();
			}
			
			if (showGrid) {
				ofDrawAxis(200);
				ofPushMatrix();
				ofRotateZ(90);
				ofDrawGridPlane(100.0f, 10);
				ofPopMatrix();
			}
			
			ofDisableDepthTest();
		}
		grabCam.end();
		
	}
	
	
	void setMouseActionsEnabled(bool flag) {
		grabCam.setMouseActionsEnabled(flag);
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
	
	
	ofxGrabCam grabCam;
	
	int cameraIndex;
	
	bool showWireframe, showGrid;
	
	
};

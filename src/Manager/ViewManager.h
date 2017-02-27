#pragma once

#include "ofxGrabCam.h"

#include "BaseManager.h"
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


class ViewManager : public BaseManager {
public:
	
	ViewManager() : cameraIndex(0) {
		
		visibility["grid"].set("Grid", true);
		visibility["wireframe"].set("Wireframe", false);
		visibility["screens"].set("Screens", true);
		visibility["stages"].set("Stages", true);
		visibility["guides"].set("Guides", true);
		visibility["cameras"].set("Cameras", true);
		
		{
			// create camera
			
			vector<ofVec3f> vs;
			
			vs.push_back(ofVec3f( 12.5, 12.5, 12.5));
			vs.push_back(ofVec3f( 12.5, 12.5, 62.5));
			vs.push_back(ofVec3f(-12.5, 12.5, 12.5));
			vs.push_back(ofVec3f(-12.5, 12.5, 62.5));
			
			vs.push_back(ofVec3f( 12.5, -12.5, 12.5));
			vs.push_back(ofVec3f( 12.5, -12.5, 62.5));
			vs.push_back(ofVec3f(-12.5, -12.5, 12.5));
			vs.push_back(ofVec3f(-12.5, -12.5, 62.5));
			
			vs.push_back(vs[0]); vs.push_back(vs[2]);
			vs.push_back(vs[1]); vs.push_back(vs[3]);
			vs.push_back(vs[4]); vs.push_back(vs[6]);
			vs.push_back(vs[5]); vs.push_back(vs[7]);
			
			vs.push_back(vs[0]); vs.push_back(vs[4]);
			vs.push_back(vs[1]); vs.push_back(vs[5]);
			vs.push_back(vs[2]); vs.push_back(vs[6]);
			vs.push_back(vs[3]); vs.push_back(vs[7]);
			cameraMesh.addVertices(vs);
			
			vs.clear();
			
			vs.push_back(ofVec3f(   11,   11,    0));
			vs.push_back(ofVec3f( 3.75, 3.75, 12.5));
			vs.push_back(ofVec3f(  -11,   11,    0));
			vs.push_back(ofVec3f(-3.75, 3.75, 12.5));
			
			vs.push_back(ofVec3f(   11,  -11,    0));
			vs.push_back(ofVec3f( 3.75,-3.75, 12.5));
			vs.push_back(ofVec3f(  -11,  -11,    0));
			vs.push_back(ofVec3f(-3.75,-3.75, 12.5));
			
			vs.push_back(vs[0]); vs.push_back(vs[2]);
			vs.push_back(vs[1]); vs.push_back(vs[3]);
			vs.push_back(vs[4]); vs.push_back(vs[6]);
			vs.push_back(vs[5]); vs.push_back(vs[7]);
			
			vs.push_back(vs[0]); vs.push_back(vs[4]);
			vs.push_back(vs[1]); vs.push_back(vs[5]);
			vs.push_back(vs[2]); vs.push_back(vs[6]);
			vs.push_back(vs[3]); vs.push_back(vs[7]);
			
			cameraMesh.addVertices(vs);
			cameraMesh.setMode(OF_PRIMITIVE_LINES);
		}
		
		font.load("Karla-Regular.ttf", 9);
		font.setLetterSpacing(1.1);
		
		
		ofLoadImage(whiteTexture, "white.png");
		
		sceneBuffer.allocate(ofGetWidth(), ofGetHeight(), GL_RGB, 4);
		screenMask.allocate(ofGetWidth(), ofGetHeight(), GL_RGB, 4);
		screenBuffer.allocate(ofGetWidth(), ofGetHeight(), GL_RGB, 4);
		alphaMaskShader.load("alpha-mask");
	
	}
	
	void setup() {
		cameraList  = new vector<CameraInfo>();
		
		resetCamera();
		
		ofAddListener(ofEvents().mousePressed, this, &ViewManager::mousePressed);
		ofAddListener(ofEvents().keyPressed, this, &ViewManager::keyPressed);
		ofAddListener(ofEvents().windowResized, this, &ViewManager::windowResized);
	}
	
	void drawImGui() {
		
		ImGui::SetNextTreeNodeOpen(isGuiOpened);
		
		if ((isGuiOpened = ImGui::CollapsingHeader("View"))) {
			
			if (ImGui::TreeNode("Cameras")) {
				
				if (ImGui::Combo("", &cameraIndex, cameraNames.c_str())) {
					if (cameraIndex >= 1) {
						applyCurrentCameraInfo();
					}
				}
				
				// Coordinates
				ImGui::Text("Coord");
					
				static float *pos;
				pos = grabCam.getGlobalPosition().getPtr();
				if (ImGui::DragFloat3("Pos", pos, 1.0f, 0.0f, 0.0f, "%.1f")) {
					grabCam.setGlobalPosition(pos[0], pos[1], pos[2]);
				}
			
				
				static float *euler;
				euler = grabCam.getOrientationEuler().getPtr();
				if (ImGui::DragFloat3("Rot", euler, 1.0f, 0.0f, 0.0f, "%.1f")) {
					static ofVec3f ne;
					ne.set(euler[0], euler[1], euler[2]);
					grabCam.setOrientation(ne);
				}
				
				static float fov;
				fov = grabCam.getFov();
				if (ImGui::SliderFloat("Fov", &fov, 0, 180)) {
					grabCam.setFov(fov);
				}
				
				bool fixUpDirection = grabCam.getFixUpDirectionEnabled();
				if (ImGui::Checkbox("Fix Up Direction", &fixUpDirection)) {
					grabCam.setFixUpDirectionEnabled(fixUpDirection);
				}
				
				ImGui::TreePop();
			}
			
			if (ImGui::TreeNode("Visibility")) {
				
				ImGui::Columns(2, "visibility_column", false);
				
				static int i;
				i = 0;
				for (auto& kv : visibility) {
					bool &val = const_cast<bool&>(kv.second.get());
					ImGui::Checkbox(kv.second.getName().c_str(), &val);
					ImGui::NextColumn();
				}
				
				ImGui::Columns(1);
				
				ImGui::TreePop();
			}
			
			ImGui::Separator();
			
			ImGui::Checkbox("Enable Screen Blending", &enableScreenBlending);
			
			ImGui::Separator();
		}
		
		if (cameraIndex >= 1) {
			
			ImOf::BeginPopup();
			ImGui::Text("%d: %s", cameraIndex, (*cameraList)[cameraIndex - 1].name.c_str());
			ImGui::SetWindowPos(ImVec2(ofGetWidth() - ImGui::GetWindowWidth() - 10 , 10));
			ImOf::EndPopup();
		}
	}
	
	void loadSettings(ofxXmlSettings &settings) {
		settings.pushTag("view");
		
		isGuiOpened = settings.getValue("isGuiOpened", isGuiOpened);
		
		settings.pushTag("visibility");
		for (auto &kv : visibility) {
			settings.getValue(kv.first, kv.second);
		}
		settings.popTag();
		
		settings.pushTag("camera");
		{
			cameraIndex = settings.getValue("index", 0);
			grabCam.setFixUpDirectionEnabled( settings.getValue("fixUpDirection", true) );
			
			if (cameraIndex == 0) {
				
				if (settings.tagExists("info")) {
					settings.pushTag("info");
					
					grabCam.setTransformMatrix( getMatrix4x4(settings, "matrix") );
					grabCam.setFov( settings.getValue("fov", 40) );
					
					settings.popTag();
				}
			
			} else if (cameraIndex <= cameraList->size()) {
				applyCurrentCameraInfo();
			}
		}
		settings.popTag();
		
		enableScreenBlending = settings.getValue("enableScreenBlending", false);
		
		settings.popTag();
	}
	
	void saveSettings(ofxXmlSettings &settings) {
		
		settings.addTag("view");
		settings.pushTag("view");
		
		settings.setValue("isGuiOpened", isGuiOpened);
		
		settings.addTag("visibility");
		settings.pushTag("visibility");
		for (auto& kv : visibility) {
			settings.setValue(kv.first, kv.second);
		}
		settings.popTag();
		
		settings.addTag("camera");
		settings.pushTag("camera");
		{
			settings.setValue("index", cameraIndex);
			settings.setValue("fixUpDirection", grabCam.getFixUpDirectionEnabled());
			
			if (cameraIndex == 0) {
				settings.addTag("info");
				settings.pushTag("info");
				
				setMatrix4x4(settings, "matrix", grabCam.getGlobalTransformMatrix());
				settings.setValue("fov", grabCam.getFov());
				
				settings.popTag();
			}
		}
		settings.popTag();
		
		settings.setValue("enableScreenBlending", enableScreenBlending);
		
		settings.popTag();
	}
	
	void updateCameraList(vector<CameraInfo> &_cameraList) {
		cameraList = &_cameraList;
		cameraIndex = 0;
		
		cameraNames = "(Custom)";
		cameraNames += '\0';
		
		static int i;
		i= 1;
		for (auto& camera : *cameraList) {
			cameraNames += ofToString(i++) + ": " + camera.name + '\0';
		}
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
		
		ofEnableAlphaBlending();
		
		grabCam.begin();
		{
			ofBackground(0);
			ofSetColor(255);
			ofEnableDepthTest();
			
			
			
			ofEnableLighting();
			cameraLight.setPosition(grabCam.getPosition());
			cameraLight.enable();
			
			if (visibility["stages"])	sceneManager.drawStages();
			if (visibility["guides"])	sceneManager.drawGuides();
			
			cameraLight.disable();
			ofDisableLighting();
			
			if (visibility["screens"]) {
				
				if (enableScreenBlending) {
					glDepthMask(false);
					ofEnableBlendMode(OF_BLENDMODE_SCREEN);
				}
				
				sourceManager.bind();
				sceneManager.drawScreens();
				sourceManager.unbind();
				
				if (enableScreenBlending) {
					ofDisableBlendMode();
					glDepthMask(true);
				}
			}
			
			// draw utils
			if (visibility["wireframe"])	sceneManager.drawWireframe();
			if (visibility["grid"])			drawGrid();
			
			ofDisableDepthTest();
			
			if (visibility["cameras"]) drawCameras();
			
		}
		grabCam.end();
		
		if (visibility["cameras"]) drawCameraLabels();
		
		ofDisableAlphaBlending();
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

		} else {
			
			switch (args.key) {
				case 'r':
					resetCamera();
					break;
				case 'w':
					visibility["wireframe"] = !visibility["wireframe"];
					break;
				case 'g':
					visibility["guides"] = !visibility["guides"];
					break;
					
			}
		}
	}
	
	void windowResized(ofResizeEventArgs & args) {
		sceneBuffer.allocate(args.width, args.height, GL_RGB, 4);
		screenMask.allocate(args.width, args.height, GL_RGB, 4);
		screenBuffer.allocate(args.width, args.height, GL_RGB, 4);
	}
	
	void drawCameras() {
		
		ofPushStyle();
		ofNoFill();
		ofSetColor(255, 192);
		
		for (auto &cameraInfo : *cameraList) {
			ofPushMatrix();
			{
				ofMultMatrix(cameraInfo.matrix);
				
				cameraMesh.draw();
				
				ofPushMatrix();
				ofRotateY(-90);
				ofDrawCircle(25, 25, 0, 12.5);
				ofDrawCircle(50, 25, 0, 12.5);
				ofPopMatrix();
			}
			ofPopMatrix();
		}
		
		ofPopStyle();
	}
	
	void drawCameraLabels() {
		
		ofPushStyle();
		ofSetColor(255, 192);
		
		for (auto &cameraInfo : *cameraList) {
			ofVec3f p = grabCam.worldToScreen( cameraInfo.matrix.getTranslation() );
			font.drawString(cameraInfo.name, p.x + 15, p.y);
		}
		
		ofPopStyle();
	}
	
	void drawGrid() {
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

	// members
	string				cameraNames;
	
	ofFbo				sceneBuffer, screenMask, screenBuffer;
	ofShader			alphaMaskShader;
	
	ofLight				cameraLight;
	
	ofMesh				cameraMesh;
	ofTrueTypeFont		font;
	
	ofxGrabCam			grabCam;
	vector<CameraInfo>	*cameraList;
	ofTexture			whiteTexture;
	
	int					cameraIndex;
	map<string, ofParameter<bool>>	visibility;
	
	bool				enableScreenBlending;
};

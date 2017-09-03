#pragma once

#include "ofxAdvancedGrabCam.h"

#include "BaseManager.h"
#include "ImGuiApp.h"
#include "CameraInfo.h"
#include "SourceManager.h"
#include "SceneManager.h"

#define CAM_INDEX_CUSTOM    0

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
        updateCameraNames();
		
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
                
                static bool buttonDisabled;
                
                ImGui::PushDisabled(cameraIndex != CAM_INDEX_CUSTOM);
                
                if (ImGui::Button("Add Camera") && cameraIndex == CAM_INDEX_CUSTOM) {
                    
                    string name = "Camera " + ofToString(cameraList.size() + 1);
                    
                    name = ofSystemTextBoxDialog("Name the new camera preset", name);
                    
                    CameraInfo camInfo;
                    camInfo.name = name;
                    camInfo.matrix = cam.getGlobalTransformMatrix();
                    camInfo.fov = cam.getFov();
                    
                    cameraList.push_back(camInfo);
                    cameraIndex = cameraList.size();
                    
                    updateCameraNames();
                }
                
                ImGui::PopDisabled();
                
                
                ImGui::SameLine();
                
                ImGui::PushDisabled(cameraIndex == CAM_INDEX_CUSTOM);
                
                if (ImGui::Button("Delete Camera") && cameraIndex != CAM_INDEX_CUSTOM) {
                        
                    cameraList.erase(cameraList.begin() + (cameraIndex - 1));
                    
                    updateCameraNames();
                    cameraIndex = CAM_INDEX_CUSTOM;
                    
                }
                
                ImGui::PopDisabled();
				
				// Coordinates
				ImGui::Text("Coord");
					
				static float *pos;
				pos = cam.getGlobalPosition().getPtr();
				if (ImGui::DragFloat3("Pos", pos, 1.0f, 0.0f, 0.0f, "%.1f")) {
					cam.setGlobalPosition(pos[0], pos[1], pos[2]);
				}
			
				
				static float *euler;
				euler = cam.getOrientationEuler().getPtr();
				if (ImGui::DragFloat3("Rot", euler, 1.0f, 0.0f, 0.0f, "%.1f")) {
					static ofVec3f ne;
					ne.set(euler[0], euler[1], euler[2]);
					cam.setOrientation(ne);
				}
				
				static float fov;
				fov = cam.getFov();
				if (ImGui::SliderFloat("Fov", &fov, 0, 180)) {
					cam.setFov(fov);
				}
				
				bool fixUpDirection = cam.getFixUpDirectionEnabled();
				if (ImGui::Checkbox("Fix Up Direction", &fixUpDirection)) {
					cam.setFixUpDirectionEnabled(fixUpDirection);
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
		
		if (cameraIndex != CAM_INDEX_CUSTOM) {
			
			ImGui::BeginCustomPopup();
			ImGui::Text("%d: %s", cameraIndex, cameraList[cameraIndex - 1].name.c_str());
			ImGui::SetWindowPos(ImVec2(ofGetWidth() - ImGui::GetWindowWidth() - 10 , 10));
			ImGui::EndCustomPopup();
		}
	}
	
	void loadSettings(ofxAdvancedXmlSettings &settings) {
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
			cam.setFixUpDirectionEnabled( settings.getValue("fixUpDirection", true) );
            
            if (settings.pushTagIfExists("current")) {
                
                cam.setTransformMatrix( settings.getValue("matrix",
                                                          cam.getGlobalTransformMatrix()));
                cam.setFov(settings.getValue("fov", cam.getFov()));
                
                settings.popTag();
            }
			
            if (settings.pushTagIfExists("list")) {
                
                int num = settings.getNumTags("info");
                
                for (int i = 0; i < num; i++) {
                    
                    settings.pushTag("info", i);
                    
                    CameraInfo camInfo;
                    camInfo.name    = settings.getValue("name", camInfo.name);
                    camInfo.matrix  = settings.getValue("matrix", camInfo.matrix);
                    camInfo.fov     = settings.getValue("fov", camInfo.fov);
                    
                    cameraList.push_back(camInfo);
                    
                    settings.popTag();
                }
                
                settings.popTag();
                
                updateCameraNames();
            }
		}
		settings.popTag();
		
		enableScreenBlending = settings.getValue("enableScreenBlending", false);
		
		settings.popTag();
	}
	
	void saveSettings(ofxAdvancedXmlSettings &settings) {
		
		settings.addPushTag("view");
		
		settings.setValue("isGuiOpened", isGuiOpened);
		
		settings.addPushTag("visibility");
		for (auto& kv : visibility) {
			settings.setValue(kv.first, kv.second);
		}
		settings.popTag();
		
		settings.addPushTag("camera");
		{
			settings.setValue("index", cameraIndex);
			settings.setValue("fixUpDirection", cam.getFixUpDirectionEnabled());
            
            settings.addPushTag("current");
            {
                settings.setValue("matrix", cam.getGlobalTransformMatrix());
                settings.setValue("fov", cam.getFov());
            }
            settings.popTag();
			
            settings.addPushTag("list");
            {
                for (auto &camInfo : cameraList) {
                    settings.addPushTag("info");
                    {
                        settings.setValue("name", camInfo.name);
                        settings.setValue("matrix", camInfo.matrix);
                        settings.setValue("fov", camInfo.fov);
                    }
                    settings.popTag();
                }
            }
            settings.popTag();
		}
		settings.popTag();
		
		settings.setValue("enableScreenBlending", enableScreenBlending);
		
		settings.popTag();
	}
	
	void importCameraList(vector<CameraInfo> &_cameraList) {
        
        for (auto &camInfo : _cameraList) {
            cameraList.push_back(camInfo);
        }
        
        updateCameraNames();
	}
    
    void updateCameraNames() {
        
        cameraNames = "(Custom)";
        cameraNames += '\0';
        
        static int i;
        i = 1;
        for (auto& camInfo : cameraList) {
            cameraNames += ofToString(i++) + ": " + camInfo.name + '\0';
        }
    }
	
	void update() {
		
		if (cameraIndex >= 1) {
			
			CameraInfo camInfo = cameraList[cameraIndex - 1];
			ofMatrix4x4 mg = cam.getGlobalTransformMatrix();
			
            // check if the camera has moved
			if ( camInfo.fov != cam.getFov() || !equalMatrix(camInfo.matrix, mg) ) {
				cameraIndex = CAM_INDEX_CUSTOM;
			}
		}
	}
	
	
	void draw(SceneManager &sceneManager, SourceManager &sourceManager) {
		
		ofEnableAlphaBlending();
		
		cam.begin();
		{
			ofBackground(0);
			ofSetColor(255);
			ofEnableDepthTest();
			
			
			
			ofEnableLighting();
			cameraLight.setPosition(cam.getPosition());
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
		cam.end();
		
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
		if (cameraIndex == CAM_INDEX_CUSTOM) {
			resetCamera();
        } else {
            CameraInfo camInfo = cameraList[cameraIndex - 1];
		
            cam.setTransformMatrix(camInfo.matrix);
            cam.setFov(camInfo.fov);
        }
	}
	
	void resetCamera() {
		cam.setFixUpDirectionEnabled(true);
		cam.resetTransform();
		cam.setPosition(300, 100, 300);
		cam.lookAt(ofVec3f(0, 0, 0), ofVec3f(0, 1, 0));
		cam.setFov(40);
		cam.setNearClip(1);
		cam.setFarClip(10000);
	}
	
	// events
	void mousePressed(ofMouseEventArgs & args) {
		
		bool guiCaptured = ImGui::GetIO().WantCaptureMouse;
		cam.setMouseActionsEnabled(!guiCaptured);
	}
	
	void keyPressed(ofKeyEventArgs & args) {
		
		int ci = args.key - '0';
		if (1 <= ci &&  ci <= cameraList.size())  {
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
		
		for (auto &camInfo : cameraList) {
			ofPushMatrix();
			{
				ofMultMatrix(camInfo.matrix);
				
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
		
		for (auto &camInfo : cameraList) {
			ofVec3f p = cam.worldToScreen( camInfo.matrix.getTranslation() );
			font.drawString(camInfo.name, p.x + 15, p.y);
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
	
	ofxAdvancedGrabCam	cam;
	vector<CameraInfo>	cameraList;
	ofTexture			whiteTexture;
	
	int					cameraIndex;
	map<string, ofParameter<bool>>	visibility;
	
	bool				enableScreenBlending;
};

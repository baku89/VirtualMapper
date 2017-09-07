#pragma once 

#include "ofMain.h"

#include "ofxFBX.h"

#include "ImGuiApp.h"
#include "BaseManager.h"
#include "CameraInfo.h"
#include "ExtendedFBXManager.h"

inline ofMatrix4x4 cameraMatrixToOf(const FbxAMatrix & matrix){
	ofMatrix4x4 m;
	
	fbxsdk::FbxVector4 t = matrix.GetT();
	fbxsdk::FbxVector4 x = matrix.GetRow(0);
	fbxsdk::FbxVector4 y = matrix.GetRow(1);
	fbxsdk::FbxVector4 z = matrix.GetRow(2);
	
	m.set(
		   z[0],  z[1],  z[2], 0,
		   y[0],  y[1],  y[2], 0,
		  -x[0], -x[1], -x[2], 0,
		   t[0],  t[1],  t[2], 1
	);
	
	return m;
}

class SceneManager : public BaseManager {
public:
	
	SceneManager()
	: showFailedModal(false)
	, isFBXLoaded(false)
    , sceneScale(1.0f)
	{}
	
    ofEvent<vector<CameraInfo>> onCameraListImport;
	
	void update() {
		if (isFBXLoaded) {
			fbxMan.update();
			fbxMan.lateUpdate();
		}
	}
	
	void draw() {
        
        ofPushMatrix();
        ofScale(sceneScale, sceneScale, sceneScale);
		
		if (isFBXLoaded) {
			fbxMan.draw();
		} else {
			ofPushMatrix();
			ofRotateX(90);
			ofDrawPlane(0, 0, 0, 100, 100);
			ofPopMatrix();
		}
        
        ofPopMatrix();
	}
	
	void drawScreens() {	drawPart(NODE_TYPE_SCREEN); }
	void drawStages() {		drawPart(NODE_TYPE_STAGE); }
	void drawGuides() {		drawPart(NODE_TYPE_GUIDE); }
	
	void drawWireframe() {
        
		ofPushStyle();
		ofSetColor(255);
		ofSetLineWidth(2);
        
        ofPushMatrix();
        ofScale(sceneScale, sceneScale, sceneScale);
		
		if (isFBXLoaded) {
			fbxMan.drawMeshWireframes();
		} else {
			ofPushMatrix();
			ofRotateZ(90);
			ofDrawGridPlane(10.0f, 5);
			ofPopMatrix();
		}
        
        ofPopMatrix();
		
		ofPopStyle();
	}
	
	void drawImGui() {
		
		ImGui::SetNextTreeNodeOpen(isGuiOpened);
		
		if ((isGuiOpened = ImGui::CollapsingHeader("Scene"))) {
			if (ImGui::Button("Open Scene")) {
				
				ofFileDialogResult result = ImGui::SystemLoadDialog("Load Scene File (.fbx)", false, "");
				
				if (result.bSuccess) {
					openFBX(result.getPath());
				}
			}
			
			ImGui::SameLine();
			ImGui::Text("%s", isFBXLoaded ? ofFilePath::getFileName(fbxScene->getFbxFilePath()).c_str() : "(No Scene)");
            
            if (cameraList.size() > 0) {
                
                if (ImGui::Button("Import Cameras")) {
                    ofNotifyEvent(onCameraListImport, cameraList, this);
                }
                
                ImGui::SameLine();
                
                ImGui::Text("Number: %lu", cameraList.size());
            }
            
            ImGui::DragFloat("Scale", &sceneScale, 0.1f, 0.0f, 1000.0f, "%.01f");
			
			ImGui::Separator();
		}
		
		// failed modal
		ImGui::Alert("Unknown File Format", "Failed to laod FBX File.", &showFailedModal);
	}
	
	vector<CameraInfo> getCameras() {
		return cameraList;
	}
	
	size_t getNumCameras() {
		return cameraList.size();
	}
	
	CameraInfo getCameraAt(size_t index) {
		return cameraList[index];
	}
	
	void loadSettings(ofxAdvancedXmlSettings &settings) {
		settings.pushTag("scene");
		
		isGuiOpened = settings.getValue("isGuiOpened", isGuiOpened);
        sceneScale = settings.getValue("scale", sceneScale);
		
		if (settings.tagExists("fbxPath")) {
			openFBX(settings.getValue("fbxPath", ""));
		}
		
		settings.popTag();
	}
	
	void saveSettings(ofxAdvancedXmlSettings &settings) {
		settings.addPushTag("scene");
		
		settings.setValue("isGuiOpened", isGuiOpened);
        settings.setValue("scale", sceneScale);
		
		if (isFBXLoaded) {
			settings.setValue("fbxPath", fbxScene->getFbxFilePath());
		}
		
		settings.popTag();
	}
	
	void dragEvent(ofDragInfo dragInfo) {
		
		if (dragInfo.files.size() == 1) {
			string path = dragInfo.files[0];
			
			if (ofFilePath::getFileExt(path) == "fbx") {
				openFBX(path);
			}
		}
	}
	
private:
	
	void openFBX(string path) {
		
		cameraList.clear();
		
		if (fbxScene != NULL) {
			delete fbxScene;
		}
		
		fbxScene = new ofxFBXScene();
		
		if (!(isFBXLoaded = fbxScene->load(path))) {
			showFailedModal = true;
			return;
		}
		
		
		fbxMan.setup( fbxScene );
		fbxMan.setAnimation(0);
		
		fbxsdk::FbxScene *fs = fbxScene->getFBXScene();
		fbxsdk::FbxNode *node = fs->GetRootNode();
		
		searchCamera(node);
	}
	
	void drawPart(int type) {
        
        ofPushMatrix();
        ofScale(sceneScale, sceneScale, sceneScale);

		if (isFBXLoaded) {
			fbxMan.drawPart(type);
		} else {
			if (type == NODE_TYPE_SCREEN) {
				ofPushMatrix();
				ofRotateX(90);
				ofDrawPlane(0, 0, 0, 100, 100);
				ofPopMatrix();
			}
		}
        
        ofPopMatrix();
	}
	
	void searchCamera(fbxsdk::FbxNode *node) {
		
		for (int i = 0; i < node->GetChildCount(); i++) {
			
			fbxsdk::FbxNode *child = node->GetChild(i);
			
			if (std::string(child->GetTypeName()) == "Camera") {
				
				fbxsdk::FbxCamera *fbxCam = child->GetCamera();
				fbxsdk::FbxAMatrix& mg = child->EvaluateGlobalTransform();
				
                CameraInfo camInfo;
				
				camInfo.name = std::string(child->GetName());
				camInfo.matrix.set( cameraMatrixToOf(mg) );
				camInfo.nearClip = fbxCam->NearPlane.Get();
				camInfo.farClip = fbxCam->FarPlane.Get();
                
                // compute field of view
                switch (fbxCam->GetApertureMode()) {
                    case fbxsdk::FbxCamera::eFocalLength:
                        fbxCam->ApertureMode.Set(fbxsdk::FbxCamera::eVertical);
                        camInfo.fov = fbxCam->ComputeFieldOfView(fbxCam->FocalLength.Get());
                        break;
                    default:
                        camInfo.fov = fbxCam->FieldOfViewY.Get();
                        break;
                }
				
				cameraList.push_back(camInfo);
			}
			
			// search grandchild
			searchCamera(child);
		}
	}
    
    float               sceneScale;
	
	bool				showFailedModal;
	
	bool				isFBXLoaded;
	
	vector<CameraInfo>	cameraList;
	ofxFBXScene			*fbxScene;
	ExtendedFBXManager	fbxMan;
};

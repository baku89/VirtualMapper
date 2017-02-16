#pragma once 

#include "ofMain.h"

#include "ofxFBX.h"

#include "ImOf.h"
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
	{}

	
	ofEvent<vector<CameraInfo>> cameraListUpdated;
	
	
	void update() {
		if (isFBXLoaded) {
			fbxMan.update();
			fbxMan.lateUpdate();
		}
	}

	
	void draw() {
		
		if (isFBXLoaded) {
			fbxMan.draw();
		} else {
			ofPushMatrix();
			ofRotateX(90);
			ofDrawPlane(0, 0, 0, 100, 100);
			ofPopMatrix();
		}
	}
	
	void drawScreens() {	drawPart(NODE_TYPE_SCREEN); }
	void drawStages() {		drawPart(NODE_TYPE_STAGE); }
	void drawGuides() {		drawPart(NODE_TYPE_GUIDE); }
	
	void drawWireframe() {
		ofPushStyle();
		ofSetColor(255);
		ofSetLineWidth(2);
		
		if (isFBXLoaded) {
			fbxMan.drawMeshWireframes();
		} else {
			ofPushMatrix();
			ofRotateZ(90);
			ofDrawGridPlane(10.0f, 5);
			ofPopMatrix();
		}
		
		ofPopStyle();
	}
	
	void drawImGui() {
		
		if (ImGui::CollapsingHeader("Scene")) {
			if (ImGui::Button("Open Scene")) {
				
				ofFileDialogResult result = ofSystemLoadDialog("Load Scene File (.fbx)", false, "");
				
				if (result.bSuccess) {
					openFBX(result.getPath());
				}
			}
			
			ImGui::SameLine();
			ImGui::Text("%s", isFBXLoaded ? ofFilePath::getFileName(fbxScene->getFbxFilePath()).c_str() : "(No Scene)");
		}
		
		// failed modal
		ImOf::Alert("Unknown File Format", "Failed to laod FBX File.", &showFailedModal);
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
	
	void loadSettings(ofxXmlSettings &settings) {
		settings.pushTag("scene");
		
		if (settings.tagExists("fbxPath")) {
			openFBX(settings.getValue("fbxPath", ""));
		}
		
		settings.popTag();
	}
	
	void saveSettings(ofxXmlSettings &settings) {
		settings.addTag("scene");
		settings.pushTag("scene");
		
		if (isFBXLoaded) {
			settings.setValue("fbxPath", fbxScene->getFbxFilePath());
		}
		
		settings.popTag();
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
		
		ofNotifyEvent(cameraListUpdated, cameraList, this);
	}
	
	void drawPart(int type) {

		if (isFBXLoaded) {
			fbxMan.drawPart(type);
		} else {
			ofPushMatrix();
			ofRotateX(90);
			ofDrawPlane(0, 0, 0, 100, 100);
			ofPopMatrix();
		}
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
				camInfo.fov = fbxCam->FieldOfViewY.Get();
				camInfo.nearClip = fbxCam->NearPlane.Get();
				camInfo.farClip = fbxCam->FarPlane.Get();
				
				cameraList.push_back(camInfo);
			}
			
			// search grandchild
			searchCamera(child);
		}
	}
	
	bool				showFailedModal;
	
	bool				isFBXLoaded;
	
	vector<CameraInfo>	cameraList;
	ofxFBXScene			*fbxScene;
	ExtendedFBXManager	fbxMan;
};

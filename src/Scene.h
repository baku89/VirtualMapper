#pragma once 

#include "ofMain.h"

#include "ofxAlembic.h"




class Scene {
public:
	
	
	Scene() : isAlembicLoaded(false) {}
	
	
	
	size_t getNumCameras() {
		return camList.size();
	}
	
	ofCamera getCamera(size_t idx) {
		return camList[idx].second;
	}
	
	string getCameraName(size_t idx) {
		return camList[idx].first;
	}

	
	void draw() {
		
		if (isAlembicLoaded) {
			abc.draw();
		} else {
			ofPushMatrix();
			ofRotateX(90);
			ofDrawPlane(0, 0, 0, 100, 100);
			ofPopMatrix();
		}
	}
	
	void drawWireframe() {
		if (isAlembicLoaded) {
			abc.debugDraw();
		} else {
			ofPushMatrix();
			ofRotateZ(90);
			ofDrawGridPlane(10.0f, 5);
			ofPopMatrix();
		}
	}
	
	void drawImGui() {
		if (ImGui::Button("Open Scene")) {
			
			ofFileDialogResult result = ofSystemLoadDialog("Load Scene File (.abc)", false, "");
			
			if (result.bSuccess) {
				openAlembic(result.getPath());
			}
		}
		
		ImGui::SameLine();
		ImGui::Text("%s", isAlembicLoaded ? ofFilePath::getFileName(abcPath).c_str() : "(No Scene)");
	}
	
	void exit() {
		abc.close();
	}
	
	string getPath() {
		return abcPath;
	}
	
	void loadSettings(ofxXmlSettings &settings) {
		settings.pushTag("scene");
		
		if (settings.tagExists("alembicPath")) {
			openAlembic(settings.getValue("alembicPath", ""));
		}
		
		settings.popTag();
	}
	
	void saveSettings(ofxXmlSettings &settings) {
		settings.addTag("scene");
		settings.pushTag("scene");
		
		if (isAlembicLoaded) {
			settings.setValue("alembicPath", abcPath);
		}
		
		settings.popTag();
	}
	
private:
	
	void openAlembic(string path) {
		
		camList.clear();
		screenList.clear();
		
		if (!(isAlembicLoaded = abc.open(path))) {
			return;
		}
		
		abcPath = path;
		
		
		for (int i = 0; i < abc.size(); i++) {
			
			ofxAlembic::IGeom *geom = abc.get(i);
			
			string type = std::string(geom->getTypeName());
			
			ofLogNotice() << "[" << i << "] " << type << " " << geom->getFullName();
			
			if (type == "Camera") {
				
				ofCamera cam;
				abc.get(i, cam);
				
				
				string name = geom->getName();
				name = name.substr(0, name.size() - 5);
				
				ofLogNotice() << "Camera Found!! " << cam.getFov() << " " << cam.getGlobalPosition();
				
				camList.push_back(pair<string, ofCamera>(name, cam));
				
			} else {
				
				ofMatrix4x4 node;
				abc.get(i, node);
				
				ofLogNotice() << "------> " << node.getTranslation();
			}
		}
	}
	
	bool				isAlembicLoaded;
	
	ofxAlembic::Reader	abc;
	string				abcPath;
	
	vector<pair<string, ofCamera>>	camList;
	vector<ofMesh>		screenList;
	vector<ofMesh>		stageList;
	
	
};

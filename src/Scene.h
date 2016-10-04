//
//  Scene.h
//  VirtualMapper
//
//  Created by Baku Hashimoto on 10/2/16.
//
//

#pragma once

#include "ofMain.h"

#include "ofxAlembic.h"




class Scene {
public:
	
	bool openWithDialog() {
		
		ofFileDialogResult result = ofSystemLoadDialog("Load Scene File (.abc)", false, ofToDataPath("."));
		if (result.bSuccess) {
			string path = result.getPath();
			if (open(path)) {
				return true;
			}
		}
		
		return false;
	}
	
	bool open(string path) {
		
		camList.clear();
		screenList.clear();
		
		if (!abc.open(path)) {
			return false;
		}
		
		abcPath = path;
		
		ofxAlembic::IGeom *geom;
		
		for (int i = 0; (geom = abc.get(i)); i++) {
			
			string type = std::string(geom->getTypeName());

			ofLogNotice() << "[" << i << "] " << type << " " << geom->getFullName();
			
			if (type == "Camera") {
				
				ofCamera cam;
				abc.get(i, cam);
				
				string name = geom->getName();
				name = name.substr(0, name.size() - 5);
				
				camList.push_back(pair<string, ofCamera>(name, cam));
			
			}
		}
		
		return true;
	}
	
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
		abc.draw();
	}
	
	void exit() {
		abc.close();
	}
	
	string getPath() {
		return abcPath;
	}
	
private:
	
	ofxAlembic::Reader	abc;
	string				abcPath;
	
	vector<pair<string, ofCamera>>	camList;
	vector<ofMesh>		screenList;
	vector<ofMesh>		stageList;
	
	
};
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
		
		if (!abc.open(path)) {
			bLoaded = false;
			return false;
		}
		
		bLoaded = true;
		abcPath = path;
		
		abc.setTime(0);
		
		
		
		for (int i = 0; i < abc.size(); i++) {
			
			ofxAlembic::IGeom *geom = abc.get(i);
			string type = std::string(geom->getTypeName());

			ofLogNotice() << "[" << i << "] " << type << " " << geom->getFullName();
			
			if (type == "Camera") {
				
				ofCamera cam;
				abc.get(i, cam);
				
				ofLogNotice() << cam.getPosition();
				
				string name = geom->getName();
				name = name.substr(0, name.size() - 5);
				
				camList.push_back(pair<string, ofCamera>(name, cam));
			
			} else if (type == "Xform") {
				
				ofMatrix4x4 mat;
				
				abc.get(i, mat);
				
				ofLogNotice() << mat;
				
			}
		}
		
		return true;
	}
	
	bool isLoaded() {
		return bLoaded;
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
	
	string getPath() {
		return abcPath;
	}
	
	void draw() {
		abc.draw();
	}
	
	void drawWireframe() {
		ofNoFill();
		ofSetColor(255, 64);
		abc.draw();
		ofFill();
		ofSetColor(255);
	}
	
	void exit() {
		abc.close();
	}
	
private:
	
	ofxAlembic::Reader	abc;
	string				abcPath;
	bool				bLoaded;
	
	vector<pair<string, ofCamera>>	camList;
	
	
};

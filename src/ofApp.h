#pragma once

#include "ofMain.h"

// addons
#include "ofxXmlSettings.h"
#include "ofxSyphon.h"
#include "ofxImGui.h"

#include "ImOf.h"
#include "CameraInfo.h"
#include "SceneManager.h"
#include "ViewManager.h"
#include "SourceManager.h"

class ofApp : public ofBaseApp{

public:
	
	// default funcs
	void setup();
	void update();
	void draw();
	void exit();
	
	void drawImGui();
	
	void keyReleased(int key);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	
	void cameraListUpdated(vector<CameraInfo> &cameraList);
	
	// variables
	
	SceneManager				sceneManager;
	ViewManager			viewManager;
	SourceManager		sourceManager;
	
	ofxImGui::Gui		gui;
	bool				showControl;
	
};

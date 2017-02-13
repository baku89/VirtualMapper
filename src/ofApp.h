#pragma once

#include "ofMain.h"

// addons
#include "ofxXmlSettings.h"
#include "ofxGrabCam.h"
#include "ofxSyphon.h"
#include "ofxImGui.h"

// my class
//#include "Camera.h"

#include "ImOf.h"

#include "Scene.h"
#include "ViewManager.h"
#include "SourceManager.h"


//#include "OSXReceiver.h"
//#include "OSXWindow.h"

// constants
#define FONT_NAME "Arial.ttf"
#define DDL_MAX_LENGTH 20
#define CAM_INDEX_DEFAULT -1
#define CAM_DEFAULT_TEXT "(default camera)"
#define CAM_KEYS_SIZE 9




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
	
//	void loadCams();
//	void saveCams();
//	void changeCam(int index);
//	void resetCam();
	
//	void setWindowOnTop(bool flag);
	
	
	// variables
	
	Scene				scene;
	ViewManager			viewManager;
	SourceManager		sourceManager;

	
	int					camIndex;
	
	
	ofxImGui::Gui		gui;
	bool				showControl;
	
	

//	char camKeys[CAM_KEYS_SIZE];
	
	// scene
	ofxGrabCam grabCam;
//	Camera prevCam;
//	vector<Camera> cams;
//	int camIndex;
//	ofVec3f camPos;
//	ofVec3f camEuler;
//    ofImage defaultTex;
};

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
#include "SharedInput.h"


//#include "OSXReceiver.h"
//#include "OSXWindow.h"

// constants
#define FONT_NAME "Arial.ttf"
#define DDL_MAX_LENGTH 20
#define CAM_INDEX_DEFAULT -1
#define CAM_DEFAULT_TEXT "(default camera)"
#define CAM_KEYS_SIZE 9

#define INPUT_IMAGE		0
#define INPUT_SYPHON	1




class ofApp : public ofBaseApp{

public:
	
	// default funcs
	void setup();
	void update();
	void draw();
	void exit();

	// event
//	void guiEvent(ofxUIEventArgs &e);
	
    // src
//    void sourceAnnouced();
//    void sourceRetired();
	
	void keyReleased(int key);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	
	// utils
//	void setGUI();
//	bool loadScreen(string file, string name);
//	void scaleScreenUV();
	
//    void alert(string message);
	
//	void loadCams();
//	void saveCams();
//	void changeCam(int index);
//	void resetCam();
	
//	void setWindowOnTop(bool flag);
	
	
	// variables
	

	ofxImGui::Gui	gui;
	
	ofTexture	defaultTex;
	
	Scene		scene;
	
	SharedInput	sharedInput;
	
	int			camIndex;
	
	int			inputSource;
	
//	bool isModalOpened;
//	bool isWindowOnTop;
//	ofTrueTypeFont font;

//	char camKeys[CAM_KEYS_SIZE];
	
    // source
//    Receiver receiver;
	
	// scene
	ofxGrabCam grabCam;
//	Camera prevCam;
//	vector<Camera> cams;
//	int camIndex;
//	ofVec3f camPos;
//	ofVec3f camEuler;
//    ofImage defaultTex;
	
	// screen
//    ofxAssimpModelLoader assimp;
//    ofMesh screen;
//    ofMesh stage;
//	vector< ofVec2f > texCoordsOrigin;
	
    // texture
//    ofMaterial material;
//    int texWidth;
//    int texHeight;
	
	// syphon
	//int syphonIndex;
	
	// display
//	bool isShowWireframe;
//    bool isShowGrid;
//	bool isFlipH, isFlipV;
	
	// ui
	/*
	ofxUICanvas *gui;
	ofxUIDropDownList *ddlInput;//, *ddlCamList;
	ofxUILabel *lblScreenName;
	ofxUINumberDialer *ndCamX, *ndCamY, *ndCamZ;
	ofxUIMinimalSlider *msCamFov, *msCamH, *msCamP, *msCamB;
	bool isGuiPressed;
	*/
    
  
    // platform
//    Window platformWindow;
	
};

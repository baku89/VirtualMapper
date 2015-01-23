#pragma once

#include "ofMain.h"

#include "ofxAssimpModelLoader.h"
#include "ofxSyphon.h"
#include "ofxUI.h"
#include "ofxXmlSettings.h"
#include "ofxGrabCam.h"

#include "Camera.h"


#define FONT_NAME "Arial.ttf"

#define DDL_MAX_LENGTH 20

#define CAM_INDEX_DEFAULT -1
#define CAM_DEFAULT_TEXT "(default camera)"
#define CAM_KEYS_SIZE 9

#define STRINGIFY(A) #A

class ofApp : public ofBaseApp{

public:
	
	// default funcs
	void setup();
	void update();
	void draw();
	void exit();

	// event
	void guiEvent(ofxUIEventArgs &e);
	
	void syphonAnnounced(ofxSyphonServerDirectoryEventArgs &arg);
	void syphonRetired(ofxSyphonServerDirectoryEventArgs &arg);
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
	
	// utils
	void setGUI();
	bool loadScreen(string file, string name);
	void scaleScreenUV(int width, int height);
	
	void loadCams();
	void saveCams();
	void changeCam(int index);
	void resetCam();
	
	void setWindowOnTop(bool flag);
	
	
	// variables
	ofxXmlSettings settings;
	bool isModalOpened;
	bool isWindowOnTop;
	ofTrueTypeFont font;

	char camKeys[CAM_KEYS_SIZE];
	
	
	// scene
	ofxGrabCam grabCam;
	Camera prevCam;
	vector<Camera> cams;
	int camIndex;
	ofVec3f camPos;
	ofVec3f camEuler;
	
	// screen
	ofxAssimpModelLoader screen;
	ofMesh mesh;
	vector< ofVec2f > texCoordsOrigin;
	ofMaterial material;
	
	
	// syphon
	ofxSyphonClient syphonClient;
	ofxSyphonServerDirectory syphonDir;
	bool isSyphonChanged;
	int syphonIndex;
	int syphonWidth;
	int syphonHeight;
	
	
	// display
	bool isShowWireframe;
	
	
	// ui
	ofxUICanvas *gui;
	ofxUIDropDownList *ddlSyphon;//, *ddlCamList;
	ofxUILabel *lblScreenName;
	ofxUINumberDialer *ndCamX, *ndCamY, *ndCamZ;
	ofxUIMinimalSlider *msCamFov, *msCamH, *msCamP, *msCamB;
	bool isGuiPressed;
	
};

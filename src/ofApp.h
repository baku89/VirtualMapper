#pragma once

#include "ofMain.h"

// addons
#include "ofxAssimpModelLoader.h"
#include "ofxSyphon.h"
#include "ofxUI.h"
#include "ofxXmlSettings.h"
#include "ofxGrabCam.h"

// my class
#include "Camera.h"

#ifdef TARGET_OSX
    #include "OSXWindow.h"
#endif

// constants
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
    
    void alert(string message);
	
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
    ofImage defaultTex;
	
	// screen
    ofxAssimpModelLoader assimp;
    ofMesh screen;
    ofMesh stage;
	vector< ofVec2f > texCoordsOrigin;
    
    // texture
    ofMaterial material;
    int texWidth;
    int texHeight;
	
	// syphon
	ofxSyphonClient syphonClient;
	ofxSyphonServerDirectory syphonDir;
	bool isSyphonChanged;
	int syphonIndex;
	
	// display
	bool isShowWireframe;
    bool isShowGrid;
	
	// ui
	ofxUICanvas *gui;
	ofxUIDropDownList *ddlSyphon;//, *ddlCamList;
	ofxUILabel *lblScreenName;
	ofxUINumberDialer *ndCamX, *ndCamY, *ndCamZ;
	ofxUIMinimalSlider *msCamFov, *msCamH, *msCamP, *msCamB;
	bool isGuiPressed;
    
    
    // platform
#ifdef TARGET_OSX
    OSXWindow platformWindow;
#endif
	
};

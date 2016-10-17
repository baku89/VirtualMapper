#pragma once

#include "ofMain.h"

// addons
#include "ofxXmlSettings.h"
#include "ofxGrabCam.h"
#include "ofxSyphon.h"
#include "ofxImGui.h"
#include "ofxTopWindow.h"

// user class
#include "ImOf.h"
#include "Scene.h"
#include "ExternalInput.h"
#include "ImageInput.h"

#define INPUT_IMAGE		0
#define INPUT_SYPHON	1

#define DEFAULT_IMAGE_PATH "default_screen.jpg"

class ofApp : public ofBaseApp{

public:
	
	// default funcs
	void setup();
	void update();
	void draw();
	void drawGui();
	void exit();
	
	void keyReleased(int key);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	
	// variables
	
	ofxImGui	gui;
	
	Scene		scene;
	
	// input
	ExternalInput	externalInput;
	ImageInput		imageInput;
	
	string		imagePath;
	
	int			camIndex;
	int			inputSource;
	bool		isWindowTop;
	bool		showGrid;
	float		gridSize;
	bool		showWireframe;
	
	// scene
	ofxGrabCam grabCam;
	
};

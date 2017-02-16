#pragma once 

#include "ofMain.h"

#include "ofxImGui.h"
#include "ofxXmlSettings.h"

class BaseSource {
public:
	
	virtual void setup() {}
	
	virtual void loadSettings(ofxXmlSettings &settings) {}
	virtual void saveSettings(ofxXmlSettings &settings) {}
	
	virtual string getName() {}
	
	virtual void bind(int textureLocation) {}
	virtual void unbind(int textureLocation) {}
	
	virtual void update() {}
	
	virtual void drawImGui() {}
	
};

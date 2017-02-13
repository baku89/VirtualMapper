#pragma once 

#include "ofMain.h"

#include "ofxImGui.h"
#include "ofxXmlSettings.h"

class SourceTexture {
public:
	
//	SourceTexture() {};
//	virtual ~SourceTexture();
//
	virtual void setup() {}
	
	virtual void loadSettings(ofxXmlSettings &settings) {}
	virtual void saveSettings(ofxXmlSettings &settings) {}
	
	virtual string getName() {}
	
	virtual void bind() {}
	virtual void unbind() {}
	
	virtual void drawImGui() {}
	
};

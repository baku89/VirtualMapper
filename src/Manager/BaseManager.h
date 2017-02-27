#pragma once

#include "ofxImGui.h"
#include "ofxXmlSettings.h"

class BaseManager {

public:
	
	BaseManager() {}
	
	virtual void setup() {}
	virtual void update() {}
	
	virtual void loadSettings(ofxXmlSettings &settings) {}
	virtual void saveSettings(ofxXmlSettings &settings) {}
	virtual void drawImGui() {}
	
	virtual void dragEvent(ofDragInfo dragInfo) {}
	
protected:
	
	bool isGuiOpened = true;
	
};

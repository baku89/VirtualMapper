#pragma once

#include "ofxImGui.h"
#include "ofxAdvancedXmlSettings.h"

class BaseManager {

public:
	
	BaseManager() {}
	
	virtual void setup() {}
	virtual void update() {}
	
	virtual void loadSettings(ofxAdvancedXmlSettings &settings) {}
	virtual void saveSettings(ofxAdvancedXmlSettings &settings) {}
	virtual void drawImGui() {}
	
	virtual void dragEvent(ofDragInfo dragInfo) {}
	
protected:
	
	bool isGuiOpened = true;
	
};

#pragma once 

#include "ofMain.h"

#include "ofxImGui.h"
#include "ofxXmlSettings.h"

class BaseSource {
public:
	
	BaseSource() {
		ofLoadImage(DefaultTexture, "default_tex.png");
	}
	
	virtual void setup() {}
	
	virtual void loadSettings(ofxXmlSettings &settings) {}
	virtual void saveSettings(ofxXmlSettings &settings) {}
	
	virtual void onActivated() {}
	virtual void onDeactivated() {}
	
	virtual void bind(int textureLocation) {}
	virtual void unbind(int textureLocation) {}
	
	virtual ofTexture& getTexture() = 0;
	
	virtual void update() {}
	
	virtual void drawImGui() {}
	
	virtual string getName() { return "";  }
	virtual bool isFlipped() { return "";  }
	
	virtual bool openPath(string path) {
		return false;
	}

protected:
	ofTexture DefaultTexture;
};

#pragma once 

#include "ofMain.h"

#include "ofxImGui.h"

class SourceTexture {
public:
	
//	SourceTexture() {};
//	virtual ~SourceTexture();
//
	virtual void setup() {};
	
	virtual string getName() {}
	
	virtual void bind() {};
	virtual void unbind() {};
	
	virtual void drawImGui() {};
	
};

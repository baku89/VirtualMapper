#pragma once

#include "ofxImGui.h"
#include "ofxXmlSettings.h"

#include "SourceManager.h"
#include "Scene.h"

class ViewManager {
public:
	
	ViewManager() {}
	
	void drawImGui() {
		
		if (ImGui::CollapsingHeader("View")) {
			
			ImGui::Checkbox("Show Wireframe", &showWireframe);
			ImGui::Checkbox("Show Grid", &showGrid);
		}

	}
	
	void loadSettings(ofxXmlSettings &settings) {
		settings.pushTag("view");
		
		showWireframe = settings.getValue("showWireframe", false);
		showGrid = settings.getValue("showGrid", showGrid);
		
		settings.popTag();
	}
	
	void saveSettings(ofxXmlSettings &settings) {
		
		settings.addTag("view");
		settings.pushTag("view");
		
		settings.setValue("showWireframe", showWireframe);
		settings.setValue("showGrid", showGrid);
		
		settings.popTag();
	}
	
	
	void draw(Scene &scene, SourceManager &sourceManager) {
		
		
		ofSetColor(255);
		
		ofEnableDepthTest();
		
		
		sourceManager.bind();
		
		scene.draw();
		
		sourceManager.unbind();
		
		if (showWireframe) {
			ofSetColor(128);
			scene.drawWireframe();
		}
		
		if (showGrid) {
			ofDrawAxis(100);
			ofPushMatrix();
			ofRotateZ(90);
			ofDrawGridPlane(100.0f, 10);
			ofPopMatrix();
		}
		
		ofDisableDepthTest();
		
	}
	
	
private:
	
	bool showWireframe, showGrid;
	
	
};

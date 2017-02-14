#pragma once 

#include "ofxImGui.h"
#include "ofxXmlSettings.h"

#include "SourceTexture.h"
#include "SourceImage.h"
#include "SourceSyphon.h"


class SourceManager {
public:
	
	SourceManager() : selected(0) {}
	
	void setup() {
		
		sources.push_back(new SourceImage());
		sources.push_back(new SourceSyphon());
		
		for (auto& source : sources) {
			source->setup();
		}
		
	}
	
	void loadSettings(ofxXmlSettings& settings) {
		settings.pushTag("sources");
		
		selected = settings.getValue("selected", 0);
		
		for (auto& source : sources) {
			source->loadSettings(settings);
		}
		settings.popTag();
	}
	
	void saveSettings(ofxXmlSettings& settings) {
		settings.addTag("sources");
		settings.pushTag("sources");
		
		settings.setValue("selected", selected);
		
		for (auto& source : sources) {
			source->saveSettings(settings);
		}
		settings.popTag();
	}
	
	
	void drawImGui() {
		
		if (ImGui::CollapsingHeader("Source", true)) {
			
			for (int i = 0; i < sources.size(); i++) {
				
				ImGui::RadioButton(sources[i]->getName().c_str(), &selected, i);
				
				if (i < sources.size() - 1) {
					ImGui::SameLine();
				}
				
			}
			
			sources[selected]->drawImGui();
		}
	}
	
	void bind(int textureLocation = 0) {
		sources[selected]->bind(textureLocation);
	}
	
	void unbind(int textureLocation = 0) {
		sources[selected]->unbind(textureLocation);
	}
	
	
private:
	
	int selected;
	
	vector<SourceTexture*> sources;
	

};

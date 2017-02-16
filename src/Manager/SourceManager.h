#pragma once 

#include "BaseManager.h"

#include "BaseSource.h"
#include "SyphonSource.h"
#include "ImageSource.h"
#include "VideoSource.h"


class SourceManager : public BaseManager {
public:
	
	SourceManager() : selected(0) {}
	
	void setup() {
		
		sources.push_back(new SyphonSource());
		sources.push_back(new ImageSource());
		sources.push_back(new VideoSource());
		
		for (auto& source : sources) {
			source->setup();
		}
		
	}
	
	void update() {
		for (auto& source : sources) {
			source->update();
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
	
	vector<BaseSource*> sources;
	

};

#pragma once 

#include "BaseManager.h"

#include "BaseSource.h"
#include "ImageSource.h"
#include "VideoSource.h"


#ifdef TARGET_OSX
#include "SyphonSource.h"
#elif defined TARGET_WIN32
#include "SpoutSource.h"
#endif

class SourceManager : public BaseManager {
public:
	
	SourceManager() : selected(0) {}
	
	void setup() {

#ifdef TARGET_OSX
		sources.push_back(new SyphonSource());
#elif defined TARGET_WIN32
		sources.push_back(new SpoutSource());
#endif
		sources.push_back(new ImageSource());
		sources.push_back(new VideoSource());
		
		for (auto& source : sources) {
			source->setup();
		}
		
	}
	
	void update() {
		sources[selected]->update();
	}
	
	void loadSettings(ofxAdvancedXmlSettings& settings) {
		settings.pushTag("sources");
		
		isGuiOpened = settings.getValue("isGuiOpened", isGuiOpened);
		
		selected = settings.getValue("selected", 0);
		
		for (auto& source : sources) {
			source->loadSettings(settings);
		}
		sources[selected]->onActivated();
		
		isFlipTexture = settings.getValue("isFlipTexture", false);
		
		settings.popTag();
	}
	
	void saveSettings(ofxAdvancedXmlSettings& settings) {
		settings.addPushTag("sources");
		
		settings.setValue("isGuiOpened", isGuiOpened);
		
		settings.setValue("selected", selected);
		
		for (auto& source : sources) {
			source->saveSettings(settings);
		}
		
		settings.setValue("isFlipTexture", isFlipTexture);
		
		settings.popTag();
	}
	
	
	void drawImGui() {
		
		ImGui::SetNextTreeNodeOpen(isGuiOpened);
		
		if ((isGuiOpened = ImGui::CollapsingHeader("Source"))) {
			
			for (int i = 0; i < sources.size(); i++) {
				
				static int prevSelected;
				
				prevSelected = selected;
				
				if (ImGui::RadioButton(sources[i]->getName().c_str(), &selected, i)) {
					sources[prevSelected]->onDeactivated();
					sources[selected]->onActivated();
				}
				
				if (i < sources.size() - 1) {
					ImGui::SameLine();
				}
				
			}
			
			sources[selected]->drawImGui();
			
			ImGui::Separator();
			
			ImGui::Checkbox("Flip Texture Vertically", &isFlipTexture);
			
			ImGui::Separator();
		}
	}
	
	void bind(int textureLocation = 0) {
		sources[selected]->bind(textureLocation);
		
		if (isFlipTexture == !sources[selected]->isFlipped()) {
			ofSetMatrixMode(OF_MATRIX_TEXTURE);
			ofPushMatrix();
			ofTranslate(0, 1);
			ofScale(1, -1);
			ofSetMatrixMode(OF_MATRIX_MODELVIEW);
		}
	}
	
	void unbind(int textureLocation = 0) {
		
		ofSetMatrixMode(OF_MATRIX_TEXTURE);
		ofPopMatrix();
		ofSetMatrixMode(OF_MATRIX_MODELVIEW);
		
		sources[selected]->unbind(textureLocation);
	}
	
	void dragEvent(ofDragInfo dragInfo) {
		
		if (dragInfo.files.size() == 1) {
			string path = dragInfo.files[0];
			
			int i = 0;
			
			for (auto& source : sources) {
				if (source->openPath(path)) {
					sources[selected]->onDeactivated();
					sources[i]->onActivated();
					
					selected = i;
					
					break;
				}
				++i;
			}
		}
	}
	
private:
	
	int selected;
	
	vector<BaseSource*> sources;
	bool				isFlipTexture;
	

};

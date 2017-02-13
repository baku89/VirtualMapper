//
//  SourceManager.h
//  VirtualMapper
//
//  Created by Baku Hashimoto on 2017/02/14.
//
//

#include "ofxImGui.h"

#include "SourceTexture.h"
#include "SourceImage.h"
#include "SourceSyphon.h"


class SourceManager {
public:
	
	SourceManager() : index(0) {}
	
	void setup() {
		
		sources.push_back(new SourceImage());
		sources.push_back(new SourceSyphon());
		
		for (auto& source : sources) {
			source->setup();
		}
		
	}
	
	
	void drawImGui() {
		
		for (int i = 0; i < sources.size(); i++) {
			
			ImGui::RadioButton(sources[i]->getName().c_str(), &index, i);
			
			if (i < sources.size() - 1) {
				ImGui::SameLine();
			}
			
		}
		
		sources[index]->drawImGui();
		
	}
	
	void bind() {
		sources[index]->bind();
	}
	
	void unbind() {
		sources[index]->unbind();
	}
	
	
private:
	
	int index;
	
	vector<SourceTexture*> sources;
	

};

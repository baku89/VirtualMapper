#pragma once

#include "ofMain.h"

#include "SourceTexture.h"

#define DEFAULT_PATH		"default_screen.jpg"
#define DEFAULT_FILENAME	"(No Image)"

class SourceImage : public SourceTexture {
public:
	
	SourceImage() : fileName(DEFAULT_FILENAME) {}
	
	void setup() {
		ofLoadImage(texture, DEFAULT_PATH);
	}
	
	string getName() {
		return "Image";
	}
	
	void bind() {
		texture.bind();
	}
	
	void unbind() {
		texture.unbind();
	}
	
	void drawImGui() {
		
		if (ImGui::Button("Load Image")) {
			
			ofFileDialogResult result = ofSystemLoadDialog("Load Image File (.abc)", false, ofToDataPath("."));
			
			if (result.bSuccess) {
				
				bool isLoaded = ofLoadImage(texture, result.getPath());
				fileName = result.getName();
				
				if (!isLoaded) {
					ofLoadImage(texture, DEFAULT_PATH);
					fileName = DEFAULT_FILENAME;
				}
				
			}
		}
		
		ImGui::SameLine();
		ImGui::Text("%s", fileName.c_str());
	}
	
private:
	
	string fileName;
	
	ofTexture texture;
};

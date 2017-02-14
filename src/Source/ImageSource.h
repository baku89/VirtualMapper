#pragma once

#include "ofMain.h"

#include "BaseSource.h"

#define DEFAULT_PATH		"default_screen.jpg"

class ImageSource : public BaseSource {
public:
	
	ImageSource() : isLoaded(false) {}
	
	void setup() {
		ofLoadImage(texture, DEFAULT_PATH);
	}
	
	void loadSettings(ofxXmlSettings &settings) {
		
		settings.pushTag("image");
		
		if (settings.tagExists("path")) {
			load(settings.getValue("path", ""));
		}
		
		settings.popTag();
		
	}
	
	void saveSettings(ofxXmlSettings &settings) {
		
		settings.addTag("image");
		settings.pushTag("image");
		
		if (isLoaded) {
			settings.setValue("path", file.getAbsolutePath());
		}
		
		settings.popTag();
	}
	
	void bind(int textureLocation) {
		texture.bind(textureLocation);
	}
	
	void unbind(int textureLocation) {
		texture.unbind(textureLocation);
	}
	
	void drawImGui() {
		
		if (ImGui::Button("Load Image")) {
			
			ofFileDialogResult result = ofSystemLoadDialog("Load Image File (.abc)", false, ofToDataPath("."));
			
			if (result.bSuccess) {
				load(result.getPath());
			}
		}
		
		ImGui::SameLine();
		ImGui::Text("%s", isLoaded ? file.getFileName().c_str() : "(No Image)");
	}
	
	//--------------------------------------------------------------
	// custom methods
	
	string getName() {
		return "Image";
	}
	
private:
	
	void load(string path) {
		isLoaded = ofLoadImage(texture, path);
		file.open(path);
		
		if (!isLoaded) {
			ofLoadImage(texture, DEFAULT_PATH);
		}
	}
	
	bool isLoaded;
	
	ofFile file;
	ofTexture texture;
};

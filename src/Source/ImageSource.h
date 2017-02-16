#pragma once

#include "ofMain.h"

#include "ImOf.h"
#include "BaseSource.h"

#define DEFAULT_PATH		"default_tex.png"

class ImageSource : public BaseSource {
public:
	
	ImageSource()
	: showFailedModal(false)
	, isLoaded(false)
	{}
	
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
	
	ofTexture& getTexture() {
		return texture;
	}
	
	void drawImGui() {
		
		if (ImGui::Button("Load Image")) {
			
			ofFileDialogResult result = ofSystemLoadDialog("Load Image File", false);
			
			if (result.bSuccess) {
				load(result.getPath());
			} else {
				showFailedModal = true;
			}
		}
		
		ImGui::SameLine();
		ImGui::Text("%s", isLoaded ? file.getFileName().c_str() : "(No Image)");
		
		ImOf::Alert("Unkown Image Foramt", "Failed to load the image as texture.", &showFailedModal);
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
			showFailedModal = true;
			ofLoadImage(texture, DEFAULT_PATH);
		}
	}
	
	bool showFailedModal;
	bool isLoaded;
	
	ofFile file;
	ofTexture texture;
	ofFbo  flippedTexture;
};

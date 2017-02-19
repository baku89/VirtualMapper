#pragma once

#include "ofMain.h"

#include "ImOf.h"
#include "BaseSource.h"

class ImageSource : public BaseSource {
public:
	
	void setup() {}
	
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
		
		if (texture.isAllocated()) {
			settings.setValue("path", file.getAbsolutePath());
		}
		
		settings.popTag();
	}
	
	void bind(int textureLocation) {
		if (texture.isAllocated()) {
			texture.bind(textureLocation);
		} else {
			DefaultTexture.bind(textureLocation);
		}
	}
	
	void unbind(int textureLocation) {
		if (texture.isAllocated()) {
			texture.unbind(textureLocation);
		} else {
			DefaultTexture.unbind(textureLocation);
		}
	}
	
	ofTexture& getTexture() {
		return texture.isAllocated() ? texture : DefaultTexture;
	}
	
	void drawImGui() {
		
		if (ImGui::Button("Load Image")) {
			
			ofFileDialogResult result = ImOf::SystemLoadDialog("Load Image File", false);
			
			if (result.bSuccess) {
				load(result.getPath());
			} else {
				showFailedModal = true;
			}
		}
		
		ImGui::SameLine();
		ImGui::Text("%s", texture.isAllocated() ? file.getFileName().c_str() : "(No Image)");
		
		ImOf::Alert("Unkown Image Foramt", "Failed to load the image as texture.", &showFailedModal);
	}
	
	//--------------------------------------------------------------
	// custom methods
	
	string getName() { return "Image"; }
	bool isFlipped() { return false; }
	
private:
	
	void load(string path) {
		bool succeed = ofLoadImage(texture, path);
		
		if (!succeed) {
			texture.clear();
			
			if (!willLoad) {
				showFailedModal = true;
			}
		} else {
			file.open(path);
		}
		
		willLoad = false;
	}
	
	bool		willLoad = true;
	bool		showFailedModal = false;
	
	ofFile		file;
	ofTexture	texture;
};

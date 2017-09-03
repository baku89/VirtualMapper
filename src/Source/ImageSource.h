#pragma once

#include "ofMain.h"
#include "ImGuiApp.h"
#include "BaseSource.h"

class ImageSource : public BaseSource {
public:
	
	void setup() {}
	
	void loadSettings(ofxXmlSettings &settings) {
		
		settings.pushTag("image");
		
		string _path = settings.getValue("path", "");
		load(_path, false);
		
		settings.popTag();
		
	}
	
	void saveSettings(ofxXmlSettings &settings) {
		
		settings.addTag("image");
		settings.pushTag("image");
		
		if (texture.isAllocated()) {
			settings.setValue("path", path);
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
			
			ofFileDialogResult result = ImGui::SystemLoadDialog("Load Image File", false);
			
			if (result.bSuccess) {
				load(result.getPath());
			}
		}
		
		ImGui::SameLine();
		ImGui::Text("%s", texture.isAllocated() ? path.c_str() : "(No Image)");
		
		ImGui::Alert("Unkown Image Foramt", "Failed to load the image as texture.", &showFailedModal);
	}
	
	bool openPath(string _path) {
		return load(_path, false);
	}
	
	//--------------------------------------------------------------
	// custom methods
	
	string getName() { return "Image"; }
	bool isFlipped() { return false; }
	
private:
	
	bool load(string _path, bool showModal = true) {
		
		bool succeed = false;
		
		if ((succeed = ofLoadImage(texture, _path))) {
			path = _path;
		
		} else {
			if (showModal) {
				showFailedModal = true;
			}
		}
		
		return succeed;
	}
	
	bool		showFailedModal = false;
	
	string		path;
	ofTexture	texture;
};

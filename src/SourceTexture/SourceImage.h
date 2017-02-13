#pragma once

#include "ofMain.h"

#include "SourceTexture.h"

class SourceImage : public SourceTexture {
public:
	
	void setup() {
		ofLoadImage(texture, "default_screen.jpg");
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
		ImGui::Text("Image!!");
	}
	
private:
	ofTexture texture;
};

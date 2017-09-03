#pragma once

#include "ofMain.h"
#include "ofxSpout2Receiver.h"
#include "ofxAdvancedXmlSettings.h"
#include "BaseSource.h"

class SpoutSource : public BaseSource {
public:

	SpoutSource() {}

	void setup() {
		receiver.setup();
	}

	void loadSettings(ofxAdvancedXmlSettings &settings) {
		settings.pushTag("spout");
		settings.popTag();
	}

	void saveSettings(ofxAdvancedXmlSettings &settings) {
		settings.addPushTag("spout");
		settings.popTag();
	}

	string getName() { return "Spout"; }
	bool isFlipped() { return false; }

	ofTexture& getTexture() {
		receiver.updateTexture();
		return receiver.getTexture();
	}

	void bind(int textureLocation) {
		receiver.updateTexture();

		if (receiver.detecting()) {
			receiver.getTexture().bind(textureLocation);
		}
		else {
			DefaultTexture.bind(textureLocation);
		}
	}

	void unbind(int textureLocation) {

		if (receiver.detecting()) {
			receiver.getTexture().unbind(textureLocation);
		}
		else {
			DefaultTexture.unbind(textureLocation);
		}
	}

	void drawImGui() {

		ImGui::PushDisabled(!receiver.detecting());
		if (ImGui::Button("Switch")) {
			receiver.showSenders();
		}
		ImGui::PopDisabled();

		ImGui::SameLine();

		if (receiver.detecting()) {
			ImGui::Text("%s", receiver.getSenderName());
		} else {
			ImGui::Text("(No Source)");
		}

	}

private:

	// member					
	ofxSpout2::Receiver			receiver;

};

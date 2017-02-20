#pragma once

#include "ofMain.h"

#include "ImOf.h"
#include "BaseSource.h"

class VideoSource : public BaseSource {
public:
	
	VideoSource() {}
	
	void setup() {
		ofAddListener(ofEvents().keyPressed, this, &VideoSource::keyPressed);
	}
	
	void loadSettings(ofxXmlSettings &settings) {
		
		settings.pushTag("video");
		
		file.open( settings.getValue("path", "") );
		if (file.exists()) {
			willOpen = true;
		}
		
		settings.popTag();
	}
	
	void saveSettings(ofxXmlSettings &settings) {
		
		settings.addTag("video");
		settings.pushTag("video");
		
		if (player.isLoaded()) {
			settings.setValue("path", file.getAbsolutePath());
		}
		
		settings.popTag();
	}
	
	void update() {
		if (willOpen) {
			load(file.getAbsolutePath());
		}
		
		if (player.isLoaded()) {
			player.update();
		}
	}
	
	ofTexture& getTexture() {
		if (player.isLoaded()) {
			return player.getTexture();
		} else {
			return std::ref(DefaultTexture);
		}
	}
	
	void bind(int textureLocation) {
		if (player.isLoaded()) {
			player.getTexture().bind(textureLocation);
		} else {
			DefaultTexture.bind();
		}
	}
	
	void unbind(int textureLocation) {
		if (player.isLoaded()) {
			player.getTexture().unbind(textureLocation);
		} else {
			DefaultTexture.unbind();
		}
	}
	
	void drawImGui() {
		
		if (ImGui::Button("Load Video")) {
			
			ofFileDialogResult result = ImOf::SystemLoadDialog("Load Video File", false, ofToDataPath("."));
			
			if (result.bSuccess) {
				load(result.getPath());
			}
		}
		
		ImGui::SameLine();
		ImGui::Text("%s", player.isLoaded() ? file.getFileName().c_str() : "(No Source)");
		
		if (player.isLoaded()) {
			float pos = player.getPosition();
			float sec = player.getDuration() * pos;
			string secText = ofToString(sec, 1) + "s";
			
			
			static int frame;
			frame = player.getCurrentFrame();
			
			if (ImOf::Seekbar("#Seekbar",&frame, 0, player.getTotalNumFrames() - 1, ImVec2(-1, 0))) {
				player.setFrame(frame);
				player.update();
			}
			
			static ImVec2 cursorPos;
			cursorPos = ImGui::GetCursorPos();
			
			static bool isPlaying;
			isPlaying = player.isPlaying();
			
			if (ImOf::PlayToggle("###PlayToggle", &isPlaying, ImVec2(30, 0))) {
				player.setPaused(!isPlaying);
			}
			
			ImGui::SameLine();
			if (ImGui::Button("Close")) {
				player.closeMovie();
			}
		}
		
		ImOf::Alert("Unkown Video Foramt", "Failed to load the video as texture.", &showFailedModal);
	}
	
	//--------------------------------------------------------------
	// custom methods
	
	string getName() { return "Video"; }
	bool isFlipped() { return false; }
	
private:
	
	void load(string path) {
		player.load(path);
		file.open(path);
		
		if (player.isLoaded()) {
			player.play();
		} else if (!willOpen) {
			showFailedModal = true;
		}
		
		willOpen = false;
	}
	
	void keyPressed(ofKeyEventArgs & args) {
		switch (args.key) {
			case ' ':
				player.setPaused(player.isPlaying());
				break;
			case OF_KEY_LEFT:
				player.setPaused(true);
				player.setFrame(max(0, player.getCurrentFrame() - 1));
				player.update();
				break;
			case OF_KEY_RIGHT:
				player.setPaused(true);
				player.nextFrame();
				player.update();
				break;
		}
	}
	
	bool showFailedModal = false;
	
	bool	willOpen = false;
	
	ofFile file;
	ofVideoPlayer player;
};

#pragma once

#include "ofMain.h"

#include "ImGuiApp.h"
#include "BaseSource.h"

class VideoSource : public BaseSource {
public:
	
	VideoSource() {}
	
	void setup() {
		ofAddListener(ofEvents().keyPressed, this, &VideoSource::keyPressed);
	}
	
	void loadSettings(ofxXmlSettings &settings) {
		
		settings.pushTag("video");
		
		path = settings.getValue("path", "");
		isWaitingOpen = true;
		
		settings.popTag();
	}
	
	void saveSettings(ofxXmlSettings &settings) {
		
		settings.addTag("video");
		settings.pushTag("video");
		
		if (player.isLoaded() || isWaitingOpen) {
			settings.setValue("path", path);
		}
		
		settings.popTag();
	}
	
	void onActivated() {
		if (isWaitingOpen) {
			load(path, false);
		}
	}
	
	void onDeactivated() {
		player.stop();
	}
	
	void update() {
		if (player.isLoaded()) {
			if (isWaitingPlay) {
				player.play();
				isWaitingPlay = false;
			}
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
			
			ofFileDialogResult result = ImGui::SystemLoadDialog("Load Video File", false, ofToDataPath("."));
			
			if (result.bSuccess) {
				load(result.getPath());
			}
		}
		
		ImGui::SameLine();
		ImGui::Text("%s", player.isLoaded() ? player.getMoviePath().c_str() : "(No Source)");
		
		if (player.isLoaded()) {
			float pos = player.getPosition();
			float sec = player.getDuration() * pos;
			string secText = ofToString(sec, 1) + "s";
			
			
			static int frame;
			frame = player.getCurrentFrame();
			
			if (ImGui::Seekbar("#Seekbar",&frame, 0, player.getTotalNumFrames() - 1, ImVec2(-1, 0))) {
				player.setFrame(frame);
				player.update();
			}
			
			static ImVec2 cursorPos;
			cursorPos = ImGui::GetCursorPos();
			
			static bool isPlaying;
			isPlaying = player.isPlaying();
			
			if (ImGui::PlayToggle("###PlayToggle", &isPlaying, ImVec2(30, 0))) {
				player.setPaused(!isPlaying);
			}
			
			ImGui::SameLine();
			if (ImGui::Button("Close")) {
				player.closeMovie();
			}
		}
		
		ImGui::Alert("Unkown Video Foramt", "Failed to load the video as texture.", &showFailedModal);
	}
	
	bool isFlipped() { return false; }
	
	bool openPath(string path) {
		return load(path, false);
	}
	
	//--------------------------------------------------------------
	// custom methods
	
	string getName() { return "Video"; }
	
	
	
private:
	
	bool load(string _path, bool showModal = true) {
		
		isWaitingOpen = false;
		
		bool succeed = false;
		
		if ((succeed = player.load(_path))) {
			isWaitingPlay = true;
			path = _path;
		} else {
			if (showModal) {
				showFailedModal = true;
			}
		}
		
		return succeed;
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
	
	bool			showFailedModal = false;
	
	bool			isWaitingOpen = false;
	bool			isWaitingPlay = false;
	string			path = "";
	
	ofVideoPlayer	player;
};

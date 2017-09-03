#pragma once

#include "ofMain.h"

#include "ImGuiApp.h"
#include "BaseSource.h"

#define SEEKBAR_WIDTH			600.0f
#define SEEKBAR_HEIGHT			41
#define SEEKBAR_MARGIN			30

#define SEEKBAR_PLAY_WIDTH		30
#define SEEKBAR_TIME_WIDTH		60

typedef enum {
    TIME = 0,
    FRAMES
} TimeDisplayMode;


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
        
        timeDisplayMode = (TimeDisplayMode)settings.getValue("timeDisplayMode", timeDisplayMode);
		
		settings.popTag();
	}
	
	void saveSettings(ofxXmlSettings &settings) {
		
		settings.addTag("video");
		settings.pushTag("video");
		
		if (player.isLoaded() || isWaitingOpen) {
			settings.setValue("path", path);
		}
        
        settings.setValue("timeDisplayMode", timeDisplayMode);
		
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
        
        if (ImGui::Button("Close")) {
            player.closeMovie();
        }
		
		if (player.isLoaded()) {
            
            static float guiWidth;
            
            guiWidth = ImGui::GetWindowSize().x;
            
            
            float ww = min((float)(ofGetWidth() - guiWidth - SEEKBAR_MARGIN * 2), SEEKBAR_WIDTH);
            
            ImVec2 pos( (guiWidth + ofGetWidth()) / 2.0f - ww / 2.0f, ofGetHeight() - SEEKBAR_HEIGHT - SEEKBAR_MARGIN);
            ImVec2 size(ww, SEEKBAR_HEIGHT);
            ImGui::SetNextWindowPos(pos);
            ImGui::SetNextWindowSize(size);
            
            ImGuiStyle& style = ImGui::GetStyle();
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 2);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.13f, 0.14f, 0.17f, 0.5f));
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
            
            const float	prevItemSpacingX = style.ItemSpacing.x;
            style.ItemSpacing.x = 16;
            
            ImGui::Begin("", NULL, ImVec2(0,0), -1.0f, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
            {
                static bool isPlaying;
                static int frame = 0;
                
                // toggle play/pause
                isPlaying = player.isPlaying();
                if (ImGui::PlayToggle("###PlayToggle", &isPlaying, ImVec2(SEEKBAR_PLAY_WIDTH, -1))) {
                    player.setPaused(!isPlaying);
                }
                
                // seek bar
                if (player.isFrameNew()) {
                    frame = player.getCurrentFrame();
                }
                
                ImGui::SameLine();
                if (ImGui::Seekbar("###Seekbar", &frame, 0, player.getTotalNumFrames() - 1, ImVec2(-SEEKBAR_TIME_WIDTH, -1))) {
                    player.setPaused(true);
                    player.setFrame(frame);
                    player.update();
                }
                
                // timecode
                static char timeDisplay[128];
                if (timeDisplayMode == FRAMES) {
                    sprintf(timeDisplay, "%dF###TimeDisplay", frame);
                } else {
                    static int time, seconds, minutes;
                    time = player.getPosition() * player.getDuration();
                    if (time < 0) time = 0;
                    seconds = time % 60;
                    minutes = time / 60;
                    sprintf(timeDisplay, "%02d:%02d###TimeDisplay", minutes, seconds);
                }
                
                ImGui::SameLine();
                if (ImGui::Button(timeDisplay, ImVec2(-1, -1))) {
                    timeDisplayMode = timeDisplayMode == TIME ? FRAMES : TIME;
                }
                
            }
            style.ItemSpacing.x = prevItemSpacingX;
            
            ImGui::End();
            ImGui::PopStyleColor(); ImGui::PopStyleColor();
            ImGui::PopStyleVar();
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
                player.previousFrame();
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
    
    TimeDisplayMode timeDisplayMode = FRAMES;
    
	string			path = "";
	
	ofVideoPlayer	player;
};

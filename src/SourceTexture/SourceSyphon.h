#pragma once


#include "ofMain.h"
#include "ofxSyphon.h"

#include "SourceTexture.h"

class SourceSyphon : public SourceTexture {
public:
	
	void setup() {
		
		client.setup();
		dir.setup();
		
		updateInputsString();
		
		ofAddListener(dir.events.serverAnnounced, this, &SourceSyphon::changed);
		ofAddListener(dir.events.serverRetired, this, &SourceSyphon::changed);
	}
	
	bool setIndex(int idx) {
		if (0 <= idx && idx < dir.size()) {
			client.set(dir.getDescription(idx));
			return true;
		} else {
			return false;
		}
	}
	
	string getName() {
		return "Syphon";
	}
	
	void bind() {
		client.draw(0, 0, 1, 1);
		client.getTexture().bind();
	}
	
	void unbind() {
		client.getTexture().unbind();
	}
	
	void drawImGui() {
		
		static int index = -1;
		
		if (ImGui::Combo("Server", &index, inputNames.c_str())) {
			ofLogNotice() << index;
			setIndex(index);
		}
		
	}
	
private:
	
	void changed(ofxSyphonServerDirectoryEventArgs &arg) {
		updateInputsString();
	}
	
	void updateInputsString() {
		
		inputNames = "";
		
		for (auto server : dir.getServerList()) {
			
			string name = server.appName;
			
			if (server.serverName != "") {
				name += " - " + server.serverName;
			}
			
			inputNames += name + '\0';
		}
	}
	
	// member
	string						inputNames;
	
	ofxSyphonClient				client;
	ofxSyphonServerDirectory	dir;
	
};

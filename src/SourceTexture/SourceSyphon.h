#pragma once


#include "ofMain.h"
#include "ofxSyphon.h"

#include "SourceTexture.h"

class SourceSyphon : public SourceTexture {
public:
	
	SourceSyphon() : index(-1) {}
	
	void setup() {
		
		client.setup();
		dir.setup();
		
		updateInputsString();
		
		ofAddListener(dir.events.serverAnnounced, this, &SourceSyphon::changed);
		ofAddListener(dir.events.serverRetired, this, &SourceSyphon::changed);
	}
	
	void loadSettings(ofxXmlSettings &settings) {
		settings.pushTag("syphon");
		
		if (settings.tagExists("inputName")) {
			
			string savedName = settings.getValue("inputName", "");
			
			int i = 0;
			for (auto server : dir.getServerList()) {
				
				string name = server.appName;
				
				if (server.serverName != "") {
					name += " - " + server.serverName;
				}
				
				if (savedName == name) {
					ofLogNotice() << "set to " << name;
					client.set(server);
					index = i;
				}
				
				i++;
			}
			
		}
		
		settings.popTag();
	}
	
	void saveSettings(ofxXmlSettings &settings) {
		settings.addTag("syphon");
		settings.pushTag("syphon");
		
		if (client.getApplicationName() != "") {
			string inputName = client.getApplicationName();
			if (client.getServerName() != "") {
				inputName += " - " + client.getServerName();
			}
			settings.addValue("inputName", inputName);
		}
		
		settings.popTag();
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
	
	void bind(int textureLocation) {
		client.draw(0, 0, 0, 0);
		client.getTexture().bind(textureLocation);
	}
	
	void unbind(int texturLocation) {
		client.getTexture().unbind(texturLocation);
	}
	
	void drawImGui() {
		if (ImGui::Combo("Server", &index, inputNames.c_str())) {
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
	int							index;
	string						inputNames;
	
	ofxSyphonClient				client;
	ofxSyphonServerDirectory	dir;
	
};

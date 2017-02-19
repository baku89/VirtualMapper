#pragma once


#include "ofMain.h"
#include "ofxSyphon.h"

#include "BaseSource.h"

class SyphonSource : public BaseSource {
public:
	
	SyphonSource() {}
	
	void setup() {
		
		client.setup();
		dir.setup();
		
		updateInputsString();
		
		ofAddListener(dir.events.serverAnnounced, this, &SyphonSource::changed);
		ofAddListener(dir.events.serverRetired, this, &SyphonSource::changed);
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
	
	string getName() { return "Syphon"; }
	bool isFlipped() { return true; }
	
	ofTexture& getTexture() {
		return client.getTexture();
	}
	
	void bind(int textureLocation) {
		client.draw(10000, 10000, 1, 1);
		if (dir.size() > 0) {
			client.getTexture().bind(textureLocation);
		} else {
			DefaultTexture.bind(textureLocation);
		}
	}
	
	void unbind(int textureLocation) {
		if (dir.size() > 0) {
			client.getTexture().unbind(textureLocation);
		} else {
			DefaultTexture.unbind(textureLocation);
		}
	}
	
	void drawImGui() {
		ImGui::PushItemWidth(-1);
		if (ImGui::Combo("Server", &index, inputNames.c_str())) {
			setIndex(index);
		}
		ImGui::PopItemWidth();
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
	int							index = -1;
	string						inputNames;
	
	ofxSyphonClient				client;
	ofxSyphonServerDirectory	dir;
	
};

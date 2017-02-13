#pragma once


#include "ofMain.h"
#include "ofxSyphon.h"

class SharedInput {
public:
	
	void setup() {
		
		client.setup();
		dir.setup();
		
		updateInputsString();
		
		ofAddListener(dir.events.serverAnnounced, this, &SharedInput::changed);
		ofAddListener(dir.events.serverRetired, this, &SharedInput::changed);
	}
	
	ofTexture getTexture() {
		return client.getTexture();
	}
	
	const vector<string> getNames() {
		return inputs;
	}
	
	bool setIndex(int idx) {
		if (0 <= idx && idx < dir.size()) {
			client.set(dir.getDescription(idx));
			return true;
		} else {
			return false;
		}
	}
	
	void bind() {
		client.draw(0, 0, 1, 1);
		client.getTexture().bind();
	}
	
	void unbind() {
		client.getTexture().unbind();
	}
	
private:
	
	void changed(ofxSyphonServerDirectoryEventArgs &arg) {
		updateInputsString();
	}
	
	void updateInputsString() {
		
		inputs.clear();
		
		for (auto server : dir.getServerList()) {
			
			string name = server.appName;
			
			if (server.serverName != "") {
				name += " - " + server.serverName;
			}
			
			inputs.push_back(name);
		}
	}
	
	// member
	vector<string>				inputs;
	
	ofxSyphonClient				client;
	ofxSyphonServerDirectory	dir;

};

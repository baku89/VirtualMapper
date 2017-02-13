//
//  ExternalInput.h
//  VirtualMapper
//
//  Created by Baku Hashimoto on 10/2/16.
//
//

#pragma once


#include "ofMain.h"
#include "ofxSyphon.h"

class ExternalInput {
public:
	
	void setup() {
		
		client.setup();
		dir.setup();
		
		ofAddListener(dir.events.serverAnnounced, this, &ExternalInput::announced);
		ofAddListener(dir.events.serverRetired, this, &ExternalInput::retired);
		
		ofLogNotice() << "server size:" << dir.getServerList().size();
		
		for (auto& server : dir.getServerList()) {
			ofLogNotice() << server.serverName << " " << server.appName;
		}
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
		client.bind();
	}
	
	void unbind() {
		client.unbind();
	}
	
private:
	
	void announced(ofxSyphonServerDirectoryEventArgs &arg) {

		refreshInputList();
		
		cout << "syphon annouced -- " << inputs.size() << endl;
	}
	
	void retired(ofxSyphonServerDirectoryEventArgs &arg) {
		
		refreshInputList();
		
		cout << "syphon retired -- " << inputs.size() << endl;
	}
	
	void refreshInputList() {
		inputs.clear();
		
		for (auto& server : dir.getServerList()) {
			inputs.push_back(server.appName);
		}
	}
	
	// member
	
	ofxSyphonClient				client;
	ofxSyphonServerDirectory	dir;
	
	vector<string>  inputs;

};

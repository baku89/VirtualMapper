//
//  SharedInput.h
//  VirtualMapper
//
//  Created by Baku Hashimoto on 10/2/16.
//
//

#pragma once


#include "ofMain.h"
#include "ofxSyphon.h"

class SharedInput {
public:
	
	void setup() {
		
		client.setup();
		dir.setup();
		
		ofAddListener(dir.events.serverAnnounced, this, &SharedInput::announced);
		ofAddListener(dir.events.serverRetired, this, &SharedInput::retired);
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
	
private:
	
	void announced(ofxSyphonServerDirectoryEventArgs &arg) {

		for (auto& dir : arg.servers) {
			ofLogNotice() << dir.appName;
			inputs.push_back( dir.appName );
		}
		
		cout << "syphon annouced -- " << inputs.size() << endl;
	}
	
	void retired(ofxSyphonServerDirectoryEventArgs &arg) {
		
		for (auto& dir : arg.servers) {
			string name = dir.appName;
			
			for (int i = inputs.size() - 1; i >= 0; i--) {
				if (inputs[i] == name) {
					inputs.erase( inputs.begin() + i );
				}
			}
		}
		
		cout << "syphon retired -- " << inputs.size() << endl;
	}
	
	// member
	
	ofxSyphonClient				client;
	ofxSyphonServerDirectory	dir;
	
	vector<string>  inputs;

};
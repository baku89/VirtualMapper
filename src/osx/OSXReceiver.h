//
//  OSXReceiver.h
//  VirtualMapper
//
//  Created by Baku Hashimoto on 2/27/15.
//
//

#pragma once

#include "ofMain.h"
#include "ofxSyphon.h"

#include <string>

class Receiver {
    
private:

    ofxSyphonClient syphonClient;
    ofxSyphonServerDirectory syphonDir;
    
    vector<string>  inputs;
    int             curtIndex;
    
    
    bool bChanged;

public:

    
    void setup() {
        
        curtIndex = 0;
        bChanged = true;
        
        syphonClient.setup();
        
        syphonDir.setup();
        ofAddListener(syphonDir.events.serverAnnounced, this, &Receiver::syphonAnnounced);
        ofAddListener(syphonDir.events.serverRetired, this, &Receiver::syphonRetired);
    }
    
    void syphonAnnounced(ofxSyphonServerDirectoryEventArgs &arg) {
        
        for (auto& dir : arg.servers) {
            inputs.push_back( dir.appName );
        }
        
        bChanged = true;
        cout << "syphon annouced -- " << inputs.size() << endl;
    }
    
    void syphonRetired(ofxSyphonServerDirectoryEventArgs &arg) {
        
        for (auto& dir : arg.servers) {
            string name = dir.appName;
            
            for (int i = inputs.size() - 1; i >= 0; i--) {
                if (inputs[i] == name) {
                    inputs.erase( inputs.begin() + i );
                }
            }
        }
        
        curtIndex = 0;
        bChanged = true;
        cout << "syphon retired -- " << inputs.size() << endl;
    }
    
    void update() {
        if (bChanged && inputs.size() > 0) {
            cout << "syphon set -- " << curtIndex << endl;
            syphonClient.set( syphonDir.getDescription(curtIndex) );
        }
    }
    
    void next() {
        bChanged = false;
    }
    
    void exit() {
        
    }
    
    bool isChanged() {
        return bChanged;
    }
    
    void bind() {
        syphonClient.bind();
    }
    
    void unbind() {
        syphonClient.unbind();
    }
    
    vector<string>& getInputs() {
        return inputs;
    }
    
    bool isEnabled() {
        return inputs.size() > 0;
    }
    
    int getWidth() {
        return syphonClient.getWidth();
    }
    
    int getHeight() {
        return syphonClient.getHeight();
    }
    
    void setInput(int index) {
        curtIndex = index;
        bChanged = true;
    }
    
    string getActiveInput() {
        return curtIndex < inputs.size() ? inputs[ curtIndex ] : "";
    }
};
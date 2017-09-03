#pragma once

#include "ofMain.h"
#include "ofxGrabCam.h"


class ofxAdvancedGrabCam : public ofxGrabCam {

public:
    
    //--------------------------
    ~ofxAdvancedGrabCam() {
        //note that older versions of oF (before october 2015) sometimes have crashes when removing listeners
        this->removeListeners();
    }
    
    void begin(ofRectangle viewport = ofGetCurrentViewport()) {
        //check listeners are in order
        if (this->inputState.listenersAttached) {
            if (!this->userSettings.listenersEnabled) {
                this->removeListeners();
            }
        }
        else {
            if (this->userSettings.listenersEnabled) {
                this->addListeners();
            }
        }
        
        glEnable(GL_DEPTH_TEST);
        this->view.viewport = viewport;
        ofCamera::begin(viewport);
        
        glGetIntegerv(GL_VIEWPORT, this->view.opengl.viewport);
        
        ofPushMatrix();
    }
    
    void addListeners() {
        ofxGrabCam::addListeners();
        ofAddListener(ofEvents().mouseScrolled, this, &ofxAdvancedGrabCam::mouseScrolled);
    }
    
    void removeListeners() {
        ofxGrabCam::removeListeners();
    }
    
    void mouseScrolled(ofMouseEventArgs & args) {
        
        static ofVec3f rp, cp;
        
        cp = this->getCursorWorld();
        rp = this->getPosition() - cp;
        
        rp *= 1 + args.scrollY / -100.0;
        
        this->setPosition(cp + rp);
    }
    
};

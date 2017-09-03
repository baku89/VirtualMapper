#pragma once

#include "ofMain.h"

#include "ofxXmlSettings.h"

class ofxAdvancedXmlSettings : public ofxXmlSettings {
public:
    
    using ofxXmlSettings::getValue;
    using ofxXmlSettings::setValue;
    
    ofVec3f getValue(const string &tag, const ofVec3f &defaultValue) {
        
        ofVec3f value(defaultValue);
        
        if (this->tagExists(tag)) {
            
            this->pushTag(tag);
            
            value.x = this->getValue("x", 0.0);
            value.y = this->getValue("y", 0.0);
            value.z = this->getValue("z", 0.0);
            
            this->popTag();
        }
        
        return value;
    };
    
    ofMatrix4x4 getValue(const string &tag, const ofMatrix4x4 &defaultValue) {
        
        ofMatrix4x4 value(defaultValue);
        
        if (this->tagExists(tag)) {
            
            float m[16];
            
            this->pushTag(tag);
            
            for (int i = 0; i < 16; i++) {
                m[i] = this->getValue("m", 0.0, i);
            }
            
            value.set(m);
            
            this->popTag();
        }
        
        return value;
    };
    
    ofQuaternion getValue(const string &tag, const ofQuaternion &defaultValue) {
        
        ofQuaternion value(defaultValue);
        
        if (this->tagExists(tag)) {
            
            this->pushTag(tag);
            
            float x, y, z, w;
            
            x = this->getValue("x", 0.0);
            y = this->getValue("y", 0.0);
            z = this->getValue("z", 0.0);
            w = this->getValue("w", 1.0);
            
            value.set(x, y, z, w);
            
            this->popTag();
        }
        
        return value;
    };
    
    void setValue(const string &tag, const ofVec3f &value) {
        
        this->addTag(tag);
        this->pushTag(tag);
        
        this->setValue("x", value.x);
        this->setValue("y", value.y);
        this->setValue("z", value.z);
        
        this->popTag();
    };
    
    void setValue(const string &tag, const ofMatrix4x4 &value) {
        
        float *m = (float *)value.getPtr();
        
        this->addTag(tag);
        this->pushTag(tag);
        
        for (int i = 0; i < 16; i++) {
            this->setValue("m", m[i], i);
        }
        
        this->popTag();
    };
    
    void setValue(const string &tag, const ofQuaternion &value) {
        
        this->addTag(tag);
        this->pushTag(tag);
        
        this->setValue("x", value.x());
        this->setValue("y", value.y());
        this->setValue("z", value.z());
        this->setValue("w", value.w());
        
        this->popTag();
    };
    
    int addPushTag(const string &tag) {
        
        int num = this->addTag(tag);
        this->pushTag(tag, num);
        
        return num;
    };
    
    bool pushTagIfExists(const string &tag) {
      
        bool exists = this->tagExists(tag);
        
        if (exists) this->pushTag(tag);
        
        return exists;
    };
    
};

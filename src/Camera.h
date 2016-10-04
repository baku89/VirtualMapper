//
//  Camera.h
//  videoMappingPreview
//
//  Created by 麦 on 1/23/15.
//
//

#pragma once

#include "ofMain.h"

class Camera {
	
public:
	string name;
	ofVec3f position;
	ofVec3f euler;
	float fov;
};
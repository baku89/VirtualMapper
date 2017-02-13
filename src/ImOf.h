#pragma once

#include "ofMain.h"
#include "ofxImGui.h"

namespace ImOf
{
	
	bool InputFloat3(string label, ofVec3f value) {
		float *v = value.getPtr();
		bool result = ImGui::InputFloat3("Position", v);
		value.set(v[0], v[1], v[2]);
		
		return result;
	}
	
	

}

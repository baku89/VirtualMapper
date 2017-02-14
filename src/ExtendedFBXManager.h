#pragma once
#include "ofxFBX.h"

class ExtendedFBXManager : public ofxFBXManager {
public:
	
	void drawScreens() { drawPart(true); }
	void drawStages() { drawPart(false); }
	
private:
	
	void drawPart(bool screen) {
		vector< shared_ptr<ofxFBXMesh> > fbxMeshes = fbxScene->getMeshes();
		for(int i = 0; i < fbxMeshes.size(); i++ ) {
			
			if ( screen == isScreenMesh(fbxMeshes[i]) ) {
				
				meshTransforms[i].transformGL();
				fbxMeshes[i]->draw( &meshes[i] );
				meshTransforms[i].restoreTransformGL();
			}
		}
	}
	
	
	inline bool isScreenMesh(shared_ptr<ofxFBXMesh> mesh) {
		return mesh->getName().compare(0, screenPrefix.size(), screenPrefix) == 0;
	}
	
	const string screenPrefix = "screen";
	
	
};

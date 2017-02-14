#pragma once
#include "ofxFBX.h"

#define NODE_TYPE_SCREEN	0
#define NODE_TYPE_STAGE		1
#define NODE_TYPE_GUIDE		2

class ExtendedFBXManager : public ofxFBXManager {
public:
	
	void drawPart(int type) {
		vector< shared_ptr<ofxFBXMesh> > fbxMeshes = fbxScene->getMeshes();
		for(int i = 0; i < fbxMeshes.size(); i++ ) {
			
			if ( type == getNodeType(fbxMeshes[i]) ) {
				
				meshTransforms[i].transformGL();
				fbxMeshes[i]->draw( &meshes[i] );
				meshTransforms[i].restoreTransformGL();
			}
		}
	}
	
private:
	
	inline int getNodeType(shared_ptr<ofxFBXMesh> mesh) {
		
		if (mesh->getName().compare(0, screenPrefix.size(), screenPrefix) == 0) {
			return NODE_TYPE_SCREEN;
		} else if (mesh->getName().compare(0, guidePrefix.size(), guidePrefix) == 0) {
			return NODE_TYPE_GUIDE;
		} else {
			return NODE_TYPE_STAGE;
		}
	}
	
	const string screenPrefix = "screen";
	const string guidePrefix = "guide";

	
	
};

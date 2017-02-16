#pragma once
#include "ofxFBX.h"

#define NODE_TYPE_SCREEN	0
#define NODE_TYPE_STAGE		1
#define NODE_TYPE_GUIDE		2

class ExtendedFBXManager : public ofxFBXManager {
public:
	
	void setup(ofxFBXScene* aScene) {
		ofxFBXManager::setup(aScene);
		
		screenMeshIndices.clear();
		stageMeshIndices.clear();
		guideMeshIndices.clear();
		
		vector< shared_ptr<ofxFBXMesh> > fbxMeshes = fbxScene->getMeshes();
		for(int i = 0; i < fbxMeshes.size(); i++ ) {
			
			switch (getNodeType(fbxMeshes[i])) {
				case NODE_TYPE_SCREEN:	screenMeshIndices.push_back(i); break;
				case NODE_TYPE_STAGE:	stageMeshIndices.push_back(i); break;
				case NODE_TYPE_GUIDE:	guideMeshIndices.push_back(i); break;
			}
		}
	}
	
	void drawPart(int type) {
		
		vector< shared_ptr<ofxFBXMesh> > fbxMeshes = fbxScene->getMeshes();
		
		vector<int> *meshIndices;
		
		switch (type) {
			case NODE_TYPE_SCREEN:	meshIndices = &screenMeshIndices; break;
			case NODE_TYPE_STAGE:	meshIndices = &stageMeshIndices; break;
			case NODE_TYPE_GUIDE:	meshIndices = &guideMeshIndices; break;
		}
		
		for(int i : *meshIndices) {
			
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
	
	vector<int> screenMeshIndices, stageMeshIndices, guideMeshIndices;
	
	const string screenPrefix = "screen";
	const string guidePrefix = "guide";

	
	
};

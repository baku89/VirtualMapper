//
//  ImageInput.h
//  VirtualMapper
//
//  Created by 麦 on 10/17/16.
//
//

#pragma once

#include "ofMain.h"

class ImageInput {
public:
	
	void load(string _path) {
		path = _path;
		ofLoadImage(texture, path);
	}
	
	string getPath() {
		return path;
	}
	
	bool isLoaded() {
		return texture.isAllocated();
	}
	
	void bind() {
		texture.bind();
	}
	
	void unbind() {
		texture.unbind();
	}
	
private:
	
	string path;
	ofTexture texture;
};

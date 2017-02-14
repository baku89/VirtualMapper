#pragma once

class CameraInfo {
public:
	CameraInfo() : name(""), matrix(), fov(40), nearClip(1), farClip(10000) {}
	
	string name;
	ofMatrix4x4 matrix;
	float fov;
	float nearClip;
	float farClip;
};

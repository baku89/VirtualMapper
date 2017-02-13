#include "ofApp.h"

//--------------------------------------------------------------
/*
void ofApp::loadCams() {
	
    if (!settings.tagExists("cameras")) {
        return;
    }
    
	settings.pushTag("cameras");
	int numOfCams = settings.getNumTags("camera");
	
	if ( numOfCams > 0 ) {
		
		for ( int i = 0; i < numOfCams; i++ ) {
			settings.pushTag("camera", i);
			
			Camera c;
			
			c.name = settings.getValue("name", "camera");
			
			c.position.x = settings.getValue("x", 0);
			c.position.y = settings.getValue("y", 0);
			c.position.z = settings.getValue("z", 0);
			
			c.euler.x = settings.getValue("h", 0);
			c.euler.y = settings.getValue("p", 0);
			c.euler.z = settings.getValue("b", 0);
			
			c.fov = settings.getValue("fov", 55);
			
			cams.push_back( c );
			
			settings.popTag();
		}
	}
	settings.popTag();

}

*/

/*
void ofApp::saveCams() {
	
	if ( !settings.tagExists("cameras") ) {
		settings.addTag("cameras");
	}
	settings.clearTagContents("cameras");
	settings.pushTag("cameras");
	
	for ( int i = 0; i < cams.size(); i++ ) {
		
		settings.addTag("camera");
		settings.pushTag("camera", i);
		
		settings.addValue("name", cams[i].name);
		settings.addValue("x", cams[i].position.x);
		settings.addValue("y", cams[i].position.y);
		settings.addValue("z", cams[i].position.z);
		settings.addValue("h", cams[i].euler.x);
		settings.addValue("p", cams[i].euler.y);
		settings.addValue("b", cams[i].euler.z);
		settings.addValue("fov", cams[i].fov);
		
		settings.popTag();
	}
	
	settings.popTag();
}
 */

/*
void ofApp::changeCam(int index) {
	
	/*
	if (index < 0 || cams.size() <= index)
		return;
	
	Camera c = cams[index];
	
	grabCam.setPosition(c.position);
	grabCam.setOrientation(c.euler);
	grabCam.setFov(c.fov);
	
	camIndex = index;
 
}*/
/*
void ofApp::resetCam() {
	
	
	grabCam.reset();
	grabCam.setFov(55);
	grabCam.setFixUpwards(true);
	grabCam.setPosition(500, 500, -500);
	
	camIndex = CAM_INDEX_DEFAULT;
 
}*/

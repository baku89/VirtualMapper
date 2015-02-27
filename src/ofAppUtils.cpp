//
//  ofAppUtils.cpp
//  videoMappingPreview
//
//  Created by 麦 on 1/20/15.
//
//

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setGUI() {
	
	vector<string> emptyList;
	
	// setup widget
	gui = new ofxUICanvas(0, 0, 200, 600);
    
    // default
    isShowGrid = true;
    isShowWireframe = true;
	isWindowOnTop = true;
	isFlipH = false;
#ifdef TARGET_OSX
	isFlipV = false;
#elif defined TARGET_WIN32
	isFlipV = true;
#endif
	
	//gui->setPadding(3);
	gui->setColorBack(ofxUIColor(80, 80, 80, 200));
	gui->setColorFillHighlight(ofxUIColor(5, 140, 190, 255));
	gui->setFont(FONT_NAME);
	
	gui->addLabel("Screen");
	lblScreenName = gui->addLabel("file:", OFX_UI_FONT_SMALL);
	gui->addLabelButton("3D LOAD", false)->setLabelText("select 3d file..");
	gui->addSpacer();

#ifdef TARGET_OSX
	gui->addLabel("Source");
	ddlInput = gui->addDropDownList("INPUT LIST", emptyList);
	ddlInput->setAllowMultiple(false);
	ddlInput->setAutoClose(true);
	ddlInput->setShowCurrentSelected(true);
	ddlInput->setLabelText("");
	gui->addSpacer();
#endif
	
	gui->addLabel("Display");
    gui->addToggle("show wireframe", &isShowWireframe);
    gui->addToggle("show gird", &isShowGrid);
#ifdef TARGET_OSX
	gui->addToggle("make window on top",&isWindowOnTop);
#endif
	gui->addWidgetDown(new ofxUIToggle("flip H", &isFlipH, 18, 18));
	gui->addWidgetRight(new ofxUIToggle("flip V", &isFlipV, 18, 18));
	gui->addSpacer();

	
	gui->addLabel("Camera");
	
	//gui->setWidgetFontSize(OFX_UI_FONT_SMALL);
	gui->addWidgetDown(new ofxUILabelButton("add cam", false));
	gui->addWidgetRight((new ofxUILabelButton("remove cam", false)));
	
	ndCamX = gui->addNumberDialer("x", -10000.0f, 10000.0f, 0.0f, 2);
	ndCamX->setName("CAMERA X");
	ndCamY = gui->addNumberDialer("y", -10000.0f, 10000.0f, 0.0f, 2);
	ndCamY->setName("CAMERA Y");
	ndCamZ = gui->addNumberDialer("z", -10000.0f, 10000.0f, 0.0f, 2);
	ndCamZ->setName("CAMERA Z");
	
	msCamH = gui->addMinimalSlider("h", -180.0f, 180.0f, 0.0f);
	msCamH->setName("CAMERA H");
	msCamP = gui->addMinimalSlider("p", -180.0f, 180.0f, 0.0f);
	msCamP->setName("CAMERA P");
	msCamB = gui->addMinimalSlider("b", -180.0f, 180.0f, 0.0f);
	msCamB->setName("CAMERA B");
	
	msCamFov = gui->addMinimalSlider("fov", 10.0f, 170.0f, 45.0f);
	msCamFov->setName("CAMERA FOV");
	
	gui->autoSizeToFitWidgets();
	gui->loadSettings("gui.xml");
	
	// set cam values
	camPos = grabCam.getPosition();
	ndCamX->setValue(camPos.x);
	ndCamY->setValue(camPos.y);
	ndCamZ->setValue(camPos.z);
	
	camEuler = grabCam.getOrientationEuler();
	msCamH->setValue(camEuler.x);
	msCamP->setValue(camEuler.y);
	msCamB->setValue(camEuler.z);
	
	ofAddListener(gui->newGUIEvent, this, &ofApp::guiEvent);
}

//--------------------------------------------------------------
bool ofApp::loadScreen(string path, string name) {

	// screen loader
	assimp.loadModel( path );
	
	if ( assimp.getMeshCount() == 0 ) {
		
		// default plane
		ofPlanePrimitive plane;
		plane.set(500, 500, 10, 10);
		screen = plane.getMesh();
		
		name = "unloaded";
	
	} else {
        
		screen = assimp.getMesh(0);
        
        stage.clear();
        
        // merge rest polygons (for stage)
        for (int i = 0; i < assimp.getNumMeshes(); i++) {
            
            ofMesh mesh = assimp.getMesh(i);
            stage.append( mesh );
        }
	}
	
	// duplicate original uv
	vector< ofVec2f >& coords = screen.getTexCoords();
	texCoordsOrigin = coords;
	
	// save settings
	lblScreenName->setLabel("file: " + name);
	settings.setValue("settings:screenPath", path);
	settings.setValue("settings:screenName", name);
	
	return assimp.getMeshCount() == 0;
}

//--------------------------------------------------------------
void ofApp::scaleScreenUV() {

	cout << "chane UV scale: " << texWidth << " * " << texHeight << " flip h=" << isFlipH << " v=" << isFlipV << endl;
	
	vector< ofVec2f >& coords = screen.getTexCoords();
	
	for ( int i = 0; i < coords.size(); i++ ) {
		
		if (isFlipH)
			coords[i].x = (1.0f - texCoordsOrigin[i].x) * texWidth;
		else
			coords[i].x = texCoordsOrigin[i].x * texWidth;

		if (isFlipV)
			coords[i].y = texCoordsOrigin[i].y * texHeight;
		else
			coords[i].y = (1.0f - texCoordsOrigin[i].y) * texHeight;
	}
}

//--------------------------------------------------------------
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

void ofApp::changeCam(int index) {
	
	if (index < 0 || cams.size() <= index)
		return;
	
	Camera c = cams[index];
	
	grabCam.setPosition(c.position);
	grabCam.setOrientation(c.euler);
	grabCam.setFov(c.fov);
	
	camIndex = index;
}

void ofApp::resetCam() {
	
	grabCam.reset();
	grabCam.setFov(55);
	grabCam.setFixUpwards(true);
	grabCam.setPosition(500, 500, -500);
	
	camIndex = CAM_INDEX_DEFAULT;
}

//--------------------------------------------------------------
void ofApp::alert(string message) {
    
    platformWindow.setWindowOnTop(false);
    ofSystemAlertDialog(message);
    platformWindow.setWindowOnTop(isWindowOnTop);
}
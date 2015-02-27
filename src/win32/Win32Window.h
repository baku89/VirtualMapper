#include "ofMain.h"
#include "ofAppGlutWindow.h"

class Window {

public:
	void setWindowOnTop(bool flag) {
		ofSetWindowAlwaysOnTop(flag);
	}
};
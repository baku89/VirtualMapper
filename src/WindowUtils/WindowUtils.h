#pragma once

#include "ofMain.h"

class WindowUtils {
	
public:
	static void setWindowOnTop(bool flag);
	static bool getWindowOnTop();
	
	static void setTitlebarTransparent(bool flag);
};
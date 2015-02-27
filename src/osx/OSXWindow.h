//
//  OSXWindow.h
//  VirtualMapper
//
//  Created by Baku Hashimoto on 2/27/15.
//
//

#ifndef VirtualMapper_OSXWindow_h
#define VirtualMapper_OSXWindow_h

#include <stdio.h>
#include <iostream>
#include <string>

#include "ofMain.h"

using namespace std;

class OSXWindow {
    
public:
    OSXWindow();
    void setWindowOnTop(bool flag);

private:
    ofAppBaseWindow * baseWindow;
    
};

#endif
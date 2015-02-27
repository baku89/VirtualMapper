//
//  OSXWindow.h
//  VirtualMapper
//
//  Created by Baku Hashimoto on 2/27/15.
//
//

#ifndef VirtualMapper_OSXWindow_h
#define VirtualMapper_OSXWindow_h

#include "ofMain.h"

class Window {
    
public:
    void setWindowOnTop(bool flag);

private:
    ofAppBaseWindow * baseWindow;
    
};

#endif
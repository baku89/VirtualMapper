//
//  MacWindow.m
//  VirtualMapper
//
//  Created by Baku Hashimoto on 2/27/15.
//
//

#include "ofxTopWindow.h"

#import <Cocoa/Cocoa.h>

 void ofxTopWindow::setWindowOnTop(bool flag) {
	 
	 NSWindow * window = (NSWindow *)ofGetWindowPtr()->getCocoaWindow();
	 
	 if ( flag ) {
		 
		 [window setLevel:CGShieldingWindowLevel()];
		 
	 } else {
		 
		 [window setLevel:NSNormalWindowLevel];
		 
	 }
}

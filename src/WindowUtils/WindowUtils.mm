#ifdef TARGET_OSX

#include "WindowUtils.h"

#import <Cocoa/Cocoa.h>

#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_COCOA
#define GLFW_EXPOSE_NATIVE_NSGL

#include <GLFW/glfw3native.h>
#import <objc/runtime.h>


@interface FakeView : NSView
@end

@implementation FakeView
- (BOOL)fakeMouseDownCanMoveWindow { return YES; }
@end


void WindowUtils::setWindowOnTop(bool flag) {
	 
	NSWindow * window = (NSWindow *)ofGetWindowPtr()->getCocoaWindow();

	if (flag) {
		[window setLevel:CGShieldingWindowLevel()];
	} else {
		[window setLevel:NSNormalWindowLevel];
	}
}

bool WindowUtils::getWindowOnTop() {


	NSWindow * window = (NSWindow *)ofGetWindowPtr()->getCocoaWindow();
	
	bool flag = window.level != NSNormalWindowLevel;
	
	ofLogNotice() << "top? = " << flag;

	return flag;
}

void WindowUtils::setTitlebarTransparent(bool flag) {
	
	NSWindow * window = (NSWindow *)ofGetWindowPtr()->getCocoaWindow();
	
	window.titlebarAppearsTransparent = flag;
	window.titleVisibility = flag ? NSWindowTitleHidden : NSWindowTitleVisible;
	window.backgroundColor = flag ? NSColor.blackColor : NSColor.whiteColor;

	static bool run_once = true;
	if (run_once)
	{
		run_once = false;
		NSView* glView = [window contentView];
		Method originalMethod = class_getInstanceMethod([glView class], @selector(mouseDownCanMoveWindow));
		Method categoryMethod = class_getInstanceMethod(FakeView.class, @selector(fakeMouseDownCanMoveWindow));
		method_exchangeImplementations(originalMethod, categoryMethod);
	}
	
}

#endif
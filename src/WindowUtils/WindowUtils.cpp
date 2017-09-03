#include "WindowUtils.h"

namespace {
	bool isWindowTop = false;
};

void WindowUtils::setWindowOnTop(bool flag) {
	HWND w32Window = ofGetWindowPtr()->getWin32Window();
	RECT rect;
	GetWindowRect(w32Window, &rect);
	SetWindowPos(w32Window, flag ? HWND_TOPMOST : HWND_NOTOPMOST, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);

	isWindowTop = flag;
}

bool WindowUtils::getWindowOnTop() {
	return isWindowTop;
}

void WindowUtils::setTitlebarTransparent(bool flag) {

}
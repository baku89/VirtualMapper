#pragma once

#include "WindowUtils.h"
#include "BaseManager.h"

class MiscManager : public BaseManager {
	
	void drawImGui() {
		
		if (ImGui::CollapsingHeader("Misc")) {
		
			if (ImGui::Checkbox("Show Window On Top", &showWindowTop)) {
				WindowUtils::setWindowOnTop(showWindowTop);
			}
		}
	}
	
	void loadSettings(ofxXmlSettings &settings) {
		
		settings.pushTag("misc");
		
		showWindowTop = settings.getValue("showWindowTop", false);
		WindowUtils::setWindowOnTop(showWindowTop);
		
		settings.popTag();
	}
	
	void saveSettings(ofxXmlSettings &settings) {
		
		settings.addTag("misc");
		settings.pushTag("misc");
		
		settings.setValue("showWindowTop", showWindowTop);
		
		settings.popTag();
	}
	
	
private:
	
	bool				showWindowTop;
	
};

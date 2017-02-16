#pragma once

#include "WindowUtils.h"
#include "BaseManager.h"

class MiscManager : public BaseManager {
	
	void drawImGui() {
		
		if (ImGui::CollapsingHeader("Misc")) {
		
			if (ImGui::Checkbox("Show Window On Top", &showWindowTop)) {
				WindowUtils::setWindowOnTop(showWindowTop);
			}
			
			if (ImGui::Button("Help")) {
				showHelp = true;
			}
			
			if (showHelp) {
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 2);
				ImGui::Begin("Help", &showHelp, ImGuiWindowFlags_AlwaysAutoResize);
				
				ImGui::Text("Left click drag: Rotate camera\n"
							"Right click drag: Zoom camera\n"
							"Hold [H] whilst left click drag: Pan camera\n"
							"[R]: Reset camera\n"
							"[C]: Toggle control panel\n"
							"[1-9]: Switch cameras in the scene file");

				ImGui::End();
				ImGui::PopStyleVar();
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
	bool				showHelp;
	
};

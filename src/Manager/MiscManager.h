#pragma once

#include "WindowUtils.h"
#include "BaseManager.h"

class MiscManager : public BaseManager {
	
	void setup() {
		ofAddListener(ofEvents().keyPressed, this, &MiscManager::keyPressed);
	}
	
	void drawImGui() {
		
		if (ImGui::CollapsingHeader("Misc")) {
		
			if (ImGui::Checkbox("Show Window On Top", &showWindowTop)) {
				WindowUtils::setWindowOnTop(showWindowTop);
			}
			
			if (ImGui::Checkbox("Enable Fullscreen", &showFullscreen)) {
				ofGetWindowPtr()->setFullscreen(showFullscreen);
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
			
			ImGui::Separator();
		}
	}
	
	void loadSettings(ofxXmlSettings &settings) {
		
		settings.pushTag("misc");
		
		showWindowTop = settings.getValue("showWindowTop", showWindowTop);
		WindowUtils::setWindowOnTop(showWindowTop);
		
		showWindowTop = settings.getValue("showFullscreen", showFullscreen);
		
		settings.popTag();
	}
	
	void saveSettings(ofxXmlSettings &settings) {
		
		settings.addTag("misc");
		settings.pushTag("misc");
		
		settings.setValue("showWindowTop", showWindowTop);
		
		settings.popTag();
	}
	
private:
	
	void keyPressed(ofKeyEventArgs & args) {
		
		switch (args.key) {
			case 'f':
				showFullscreen = !showFullscreen;
				ofGetWindowPtr()->setFullscreen(showFullscreen);
				break;
		}
	}
	
	bool				showWindowTop = false;
	bool				showFullscreen = false;
	bool				showHelp;
	
};

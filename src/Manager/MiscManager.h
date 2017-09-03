#pragma once

#include "WindowUtils.h"
#include "BaseManager.h"

class MiscManager : public BaseManager {
	
	void setup() {
		ofAddListener(ofEvents().keyPressed, this, &MiscManager::keyPressed);
	}
	
	void drawImGui() {
		
		ImGui::SetNextTreeNodeOpen(isGuiOpened);
		
		if ((isGuiOpened = ImGui::CollapsingHeader("Misc"))) {
		
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
				
				static const vector<pair<string, string>> helpTexts = {
					{"Left click drag",			"Rotate camera"},
					{"Right click drag",		"Zoom camera"},
					{"Middle click drag",		"Pan camera"},
					{"[H] + Left click drag",	""},
					{"[R]",						"Reset camera"},
					{"[1-9]",					"Switch cameras in the scene file"},
					{"", ""},
					{"[F]",						"Toggle fullscreen"},
					{"[C]",						"Toggle control panel"},
					{"[G]",						"Toggle showing guides"},
					{"[W]",						"Toggle showing wireframe"}
				};
				
				static stringstream sskey, ssdesc;
				
				sskey.str("");
				ssdesc.str("");
				
				for (auto& line : helpTexts) {
					sskey	<< line.first << "\n";
					ssdesc	<< line.second << "\n";
				}
				
				ImGui::Columns(2, NULL, true);
				
				ImGui::Text("%s", sskey.str().c_str());
				ImGui::NextColumn();
				ImGui::Text("%s", ssdesc.str().c_str());
				
				ImGui::Columns(1);

				ImGui::End();
				ImGui::PopStyleVar();
			}
			
			ImGui::Separator();
		}
	}
	
	void loadSettings(ofxAdvancedXmlSettings &settings) {
		
		settings.pushTag("misc");
		
		isGuiOpened = settings.getValue("isGuiOpened", isGuiOpened);
		
		showWindowTop = settings.getValue("showWindowTop", showWindowTop);
		WindowUtils::setWindowOnTop(showWindowTop);
		
		showWindowTop = settings.getValue("showFullscreen", showFullscreen);
		
		settings.popTag();
	}
	
	void saveSettings(ofxAdvancedXmlSettings &settings) {
		
		settings.addTag("misc");
		settings.pushTag("misc");
		
		settings.setValue("isGuiOpened", isGuiOpened);
		
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

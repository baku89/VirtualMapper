#pragma once

#include "ofMain.h"
#include "ofxImGui.h"

namespace ImOf
{
	inline void SetFont() {
		ImGuiIO * io = &ImGui::GetIO();
		ImFontConfig font_config;
		font_config.OversampleH = 1;
		font_config.OversampleV = 1;
		io->Fonts->AddFontFromFileTTF(&ofToDataPath("Karla-Regular.ttf")[0], 14.f, &font_config);
	}
	
	inline void SetStyle() {
		ImGuiStyle *style = &ImGui::GetStyle();
		style->WindowRounding = 0;
		style->ScrollbarSize = 3;
		style->ScrollbarRounding = 0;
		style->FramePadding = ImVec2(6, 4);
		style->ItemSpacing = ImVec2(4, 8);
		style->FrameRounding = 2;
		style->GrabRounding = 2;
	}
	
	inline void BeginTransparentWindow() {
		static bool p_open = true;
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
		ImGui::Begin("Show Control", &p_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	}
	
	inline void EndTransparentWindow() {
		ImGui::End();
		ImGui::PopStyleColor();
	}
	
	inline void BeginPopup() {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 2);
		ImGui::Begin("Long title", NULL, ImVec2(0,0), -1.0f, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
	}
	
	inline void EndPopup() {
		ImGui::End();
		ImGui::PopStyleVar();
	}
}

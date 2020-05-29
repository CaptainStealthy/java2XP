#pragma once

#include <JNIWrapper/ImGUIIntegration.h>
#include <iostream>
#include <string>
#include <utility/Logger.h>

#include "imgui.h"

using namespace std;
using namespace flwnd;

class JavaXPWindow : public ImGUIWindow {


public:
	
	JavaXPWindow(int width, int height, int decoration, string windowTitle);
	~JavaXPWindow();
	void drawGui(ImGUIWindow& window);
	void appendCharacterToWindow(string character);
	void logError(std::string error);
protected:


private:
	bool windowOpen = true;
	ImGuiTextBuffer logBuffer;
	unique_ptr<Logger> logger;
	int lines = 0;

};

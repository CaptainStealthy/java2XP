#include "javaXPWindow.h"


JavaXPWindow::JavaXPWindow(int width, int height, int decoration, string windowTitle) : ImGUIWindow(width, height, decoration)
{
	logger = make_unique<Logger>("Java XP Window");
	setTitle(windowTitle.c_str());

	setBuildCallback([&](ImGUIWindow & window) {
		this->drawGui(window);
	});

	setErrorHandler([&](const string error) {
		this->logError(error);
	});
}

JavaXPWindow::~JavaXPWindow()
{
	logBuffer.clear();
	cout << "Culprit \n";
}

void JavaXPWindow::drawGui(ImGUIWindow & window)
{


	if (ImGui::Button("Clear")) {
		logBuffer.clear();
	}

	ImGui::BeginChild("LogWindow");

	ImGui::TextUnformatted(logBuffer.begin(), logBuffer.end());
	
	ImGui::EndChild();


}

void JavaXPWindow::appendCharacterToWindow(string character)
{
	logBuffer.append(character.c_str());
}

void JavaXPWindow::logError(std::string error)
{
	logger->setLogLevel(Logger::log_error);
	logger->logString(error);
}

#pragma once
#include <string>
class graphicsWrapper
{
public:
	graphicsWrapper();
	void createBundle();
	void endBundle();
	bool enabled = false;
	static void transmitToLua();
	void startGraphicsAPI();
	void stopGraphicsAPI();
	void addDrawCommand(std::string command);
	~graphicsWrapper();
};


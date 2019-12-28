#pragma once
#include<XPLMGraphics.h>
#include<XPLMDisplay.h>
class RainEffects
{
public:
	

	void initLibrain();

	void cleanup();

	void registerCallBack();
	~RainEffects();
	RainEffects();
	
private:
	
	void initGlassVariables();

	
	
};


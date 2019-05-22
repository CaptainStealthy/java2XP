#pragma once
#include<XPLMGraphics.h>
#include<XPLMDisplay.h>
class RainEffects
{
public:
	static RainEffects *rainEffects;
	
	static RainEffects* getInstance();
	

	void initLibrain();

	void cleanup();

	void registerCallBack();

	
private:
	
	~RainEffects();
	RainEffects();
	
};


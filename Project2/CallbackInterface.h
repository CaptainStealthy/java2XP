#pragma once
#include <XPLMProcessing.h>
#include <functional>
#include <map>
#include <memory>

using namespace std;

class CallbackInterface
{
public:
	CallbackInterface();

	map<int,function<void()>> callbacksRegistered;

	int registerCB(std::function<void()> func);

	void removeCallback(int id);

	static float callback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void * inRefcon);

	void stop();

	
	~CallbackInterface();

	static shared_ptr<CallbackInterface> makeNewInterface();

	static shared_ptr<CallbackInterface> getInstance();

private:
	static shared_ptr<CallbackInterface> callbackInterface;
};


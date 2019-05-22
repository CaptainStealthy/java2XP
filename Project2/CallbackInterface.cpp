#include "CallbackInterface.h"



shared_ptr<CallbackInterface> CallbackInterface::callbackInterface = nullptr;

CallbackInterface::CallbackInterface()
{
	XPLMRegisterFlightLoopCallback(callback, -1.0, NULL);
}

int CallbackInterface::registerCB(std::function<void()> func)
{
	int thisId = callbacksRegistered.size() + 1;
	
	callbacksRegistered.insert(std::make_pair(thisId, func));
	return thisId;
}

void CallbackInterface::removeCallback(int id)
{
	callbacksRegistered.erase(id);
}

float CallbackInterface::callback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void * inRefcon)
{
	for (auto func : callbackInterface->callbacksRegistered) {
		func.second();
	}

	return -1.0f;
}

void CallbackInterface::stop()
{
	callbacksRegistered.clear();
	callback(0.0f, 0.0f, 0.0f, NULL);
	callbackInterface = nullptr;
}

CallbackInterface::~CallbackInterface()
{
	XPLMUnregisterFlightLoopCallback(callback, NULL);
}

shared_ptr<CallbackInterface> CallbackInterface::makeNewInterface()
{
	callbackInterface = make_shared<CallbackInterface>();
	return callbackInterface;
}

shared_ptr<CallbackInterface> CallbackInterface::getInstance()
{
	return callbackInterface;
}

#pragma once

#include <XPLMUtilities.h>
#include <vector>
#include <map>
#include <memory>
#include "JNIWrapper/JNIWrapper.h"

class JNIWrapper;

class CommandInterface
{
public:

	CommandInterface();
	~CommandInterface();

	int findCommand(std::string simLocation);
	int createNewCommand(std::string simLocation, std::string description);
	int registerCommandCallbackHandler(int commandID, int inBefore);
	void unregisterCommandCallbackHandler(int commandID, int inBefore,int callbackID);
	
	static int registeredCallback (XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void * inRefcon);

private:

	std::map<int,XPLMCommandRef> commandIDsRegistered;
	std::vector<int> commandCalbackIDsregistered;
	

	
};

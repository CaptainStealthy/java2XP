#include "CommandInterface.h"





CommandInterface::CommandInterface()
{
	
}

CommandInterface::~CommandInterface()
{
}

int CommandInterface::findCommand(std::string simLocation)
{

	XPLMCommandRef commandFound = XPLMFindCommand(simLocation.c_str());

	if(commandFound == NULL)
	{
		return -1;
	}

	int commandIDToAssign = commandIDsRegistered.size() + 1;

	commandIDsRegistered.insert(std::make_pair(commandIDToAssign, commandFound));

	return commandIDToAssign;
	
}

int CommandInterface::createNewCommand(std::string simLocation, std::string description)
{

	XPLMCommandRef newCommand = XPLMCreateCommand(simLocation.c_str(), description.c_str());

	int commandIDToAssign = commandIDsRegistered.size() + 1;

	commandIDsRegistered.insert(std::make_pair(commandIDToAssign, newCommand));

	return commandIDToAssign;
	
}

int CommandInterface::registerCommandCallbackHandler(int commandID, int inBefore)
{

	XPLMCommandRef commandToRegisterWith = commandIDsRegistered.at(commandID);
	int callbackIDToAssign = commandCalbackIDsregistered.size() + 1;

	XPLMRegisterCommandHandler(commandToRegisterWith, registeredCallback, inBefore, reinterpret_cast<void*>(callbackIDToAssign));

	return callbackIDToAssign;
	
}

void CommandInterface::unregisterCommandCallbackHandler(int commandID, int inBefore,int callbackID)
{

	XPLMCommandRef commandRegistered = commandIDsRegistered.at(commandID);

	XPLMUnregisterCommandHandler(commandRegistered, registeredCallback, inBefore, reinterpret_cast<void*>(callbackID));

	
	
}


int CommandInterface::registeredCallback(XPLMCommandRef inCommand, XPLMCommandPhase inPhase, void* inRefcon)
{

	int commandCallbackID = reinterpret_cast<int>(inRefcon);
	
	int commandPhase = static_cast<int>(inPhase);

	return PluginRunner::getGlobalPlugin()->getJNI()->callCommandCallback(commandCallbackID, commandPhase);

	
}

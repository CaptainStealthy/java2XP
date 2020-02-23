#include "JNICommands.h"

int JNICommands::findCommand(JNIEnv* e, jobject o, jstring simLocation)
{

	string commandName = e->GetStringUTFChars(simLocation, nullptr);
	return getCommandInterface()->findCommand(commandName);
	
}

int JNICommands::createCommand(JNIEnv* e, jobject o, jstring simLocation, jstring description)
{

	string commandName = e->GetStringUTFChars(simLocation, nullptr);
	string commandDescription = e->GetStringUTFChars(description, nullptr);

	return getCommandInterface()->createNewCommand(commandName, commandDescription);
	
}

int JNICommands::registerCommandHandler(JNIEnv* e, jobject o, jint commandID, jint inBefore)
{

	return getCommandInterface()->registerCommandCallbackHandler(commandID, inBefore);
	
}

void JNICommands::unregisterCommandHandler(JNIEnv* e, jobject o, jint commandID, jint callbackID, jint inBefore)
{
	getCommandInterface()->unregisterCommandCallbackHandler(commandID, inBefore, callbackID);
}

std::shared_ptr<CommandInterface> JNICommands::getCommandInterface()
{
	return PluginRunner::getGlobalPlugin()->getCommandInterface();
}

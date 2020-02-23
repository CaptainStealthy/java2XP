#pragma once

#include "jni.h"
#include "JNIWrapper.h"
#include <string>
#include "commands/CommandInterface.h"


namespace JNICommands
{

	extern int findCommand(JNIEnv * e, jobject o, jstring simLocation);
	extern int createCommand(JNIEnv * e, jobject o, jstring simLocation, jstring description);
	extern int registerCommandHandler(JNIEnv * e, jobject o, jint commandID, jint inBefore);
	extern void unregisterCommandHandler(JNIEnv * e, jobject o, jint commandID, jint callbackID, jint inBefore);
	extern std::shared_ptr<CommandInterface> getCommandInterface();

	
}
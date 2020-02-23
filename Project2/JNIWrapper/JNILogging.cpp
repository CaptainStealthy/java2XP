#include "JNILogging.h"

void JNILogging::appendChar(JNIEnv* e, jobject o, jstring character)
{
	const std::string value = e->GetStringUTFChars(character, nullptr);
	if (getCurrentJNI() && getCurrentJNI()->getWindowPtr()) {
		getCurrentJNI()->getWindowPtr()->appendCharacterToWindow(value);
	}
	XPLMDebugString(value.c_str());
}

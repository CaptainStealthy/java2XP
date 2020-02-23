#pragma once
#include "jni.h"
#include <string>
#include "globalSingletons/PluginRunner.h"

namespace JNIModules
{
	extern jint createNewModule(JNIEnv * e, jobject o, jstring name);

	jint createNewGraphicsModule(JNIEnv * e, jobject o, jstring name, jint x, jint y, jint height, jint width);

	void setGraphicsModule(JNIEnv * e, jobject o, jint moduleID);

	void enableGraphicsBackend(JNIEnv * e, jobject o, jint x, jint y);
}

#pragma once

#include "jni.h"
#include <string>
#include <JNIWrapper/JNIUtils.h>

using namespace jniutils;

namespace JNILogging
{
	extern void appendChar(JNIEnv * e, jobject o, jstring character);

}

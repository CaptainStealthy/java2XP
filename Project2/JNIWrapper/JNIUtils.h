#pragma once
#include <XPLMUtilities.h>
#include <string>
#include "globalSingletons/GlobalStore.h"
#include "jni.h"
#include <memory>
#include "../globalSingletons/PluginRunner.h"

using namespace std;

class JNIWrapper;

namespace jniutils {

	extern string getXplanePath();

	extern string getProjectPath();

	extern string getAircraftPath();

	extern jstring getProjectDirectory(JNIEnv * e, jobject o);

	extern std::shared_ptr<JNIWrapper> getCurrentJNI();

	extern jstring getXPlaneDirectory(JNIEnv * e, jobject o);

	extern jstring getAircraftDirectory(JNIEnv * e, jobject o);

} 

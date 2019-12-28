#pragma once
#include <string>
#include "globalSingletons/drefStore.h"
#include "globalSingletons/GlobalStore.h"
#include <jni.h>
#include <jvmti.h>
#include <map>
#include <iostream>
#include <utility/Logger.h>
#include "globalSingletons/dref.h"
#include "CallbackInterface.h"
#include "javaXPWindow.h"
#include "globalSingletons/PluginRunner.h"
#include "XPLMDisplay.h"
#include <XPLMMenus.h>


using namespace std;
class PluginRunner;

class JNIWrapper {

public:

	JNIWrapper();
	~JNIWrapper();

	void windowExitedCB() {
		windowExited = true;
		delete window;
		window = nullptr;
	}

	void setInitialized(bool state) {

		this->jvmInitialized = state;

		CallbackInterface::getInstance()->registerCB(std::bind(&JNIWrapper::update, this));

	}

private:

	bool javaInitialized = false;

	XPLMCommandRef logWindowCommand;
	XPLMMenuID ourMenu;

	bool jvmInitialized = false;
	bool makeWindow = false;
	bool windowExited = true;

	bool drefsInitialized = false;
	
	JavaXPWindow* window;

	JavaVM* javaVM;
	JNIEnv* env;

	std::shared_ptr<DrefStore> instance;

    Logger* logger;

	map<string, jmethodID> methods;

	map<string, jclass> classes;
	
	string jarDirectory;

	bool startJVM();

	void update();

	bool findLoadedJVM(std::string jarPath);

	void addSystemClassLoaderPath(std::string classPath);

	void stop();

	bool registerNatives();

	friend void registerDref(JNIEnv *e, jobject o, jstring name, jint type);

	void findAndAddMethod(string methodName, string className, string signature);

	void findAndAddStaticMethod(string methodName, string className, string signature);

	void findAndLoadClass(string className);

	friend jfloat getFloatRef(JNIEnv* e, jobject o, jstring name);

	friend jint getIntRef(JNIEnv*e, jobject o, jstring name);

	friend jstring getStringRef(JNIEnv *e, jobject o, jstring name);

	friend jfloatArray getFloatArrayRef(JNIEnv *e, jobject o, jstring name);

	friend jintArray getIntArrayRef(JNIEnv *e, jobject o, jstring name);

	friend void setFloatRef(JNIEnv*e, jobject o, jstring name, jfloat value);

	friend void setIntRef(JNIEnv*e, jobject o, jstring name, jint value);

	friend void setStringRef(JNIEnv*e, jobject o, jstring name, jstring value);

	friend void setFloatArrayRef(JNIEnv * e, jobject o, jstring name, jobjectArray values);

	friend void setIntArrayRef(JNIEnv * e, jobject o, jstring name, jobjectArray values);

	friend void appendChar(JNIEnv * e, jobject o, jstring character);

	friend int drawWindow(XPLMDrawingPhase     inPhase,
		int                  inIsBefore,
		void *               inRefcon);

	friend int showWindow(XPLMCommandRef       inCommand, XPLMCommandPhase     inPhase, void *               inRefcon);
};
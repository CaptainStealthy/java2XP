#pragma once
#include <string>
#include "Datarefs/drefStore.h"
#include "globalSingletons/GlobalStore.h"
#include <jni.h>
#include <jvmti.h>
#include <map>
#include <iostream>
#include <utility/Logger.h>
#include "Datarefs/Dref.h"
#include "CallbackInterface.h"
#include "window/javaXPWindow.h"
#include "globalSingletons/PluginRunner.h"
#include "XPLMDisplay.h"
#include <XPLMMenus.h>
#include "JNIWrapper/JNIDrefs.h"


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

	JavaXPWindow* getWindowPtr()
	{
		return this->window;
	}

	int callCommandCallback(int callbackID, int phaseID);
	
private:

	bool javaInitialized = false;

	XPLMCommandRef logWindowCommand;
	XPLMMenuID ourMenu;

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
	
	void initializeWindow();

	void update();

	bool findLoadedJVM(std::string jarPath);

	void addSystemClassLoaderPath(std::string classPath);

	void stop();

	bool registerNatives();

	friend void registerDref(JNIEnv *e, jobject o, jstring name, jint type);

	void findAndAddMethod(string methodName, string className, string signature);

	void findAndAddStaticMethod(string methodName, string className, string signature);

	void findAndLoadClass(string className);

	void initializeClasses();

	friend int drawWindow(XPLMDrawingPhase     inPhase,
		int                  inIsBefore,
		void *               inRefcon);

	friend int showWindow(XPLMCommandRef       inCommand, XPLMCommandPhase     inPhase, void * inRefcon);
};
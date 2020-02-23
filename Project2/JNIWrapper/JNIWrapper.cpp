#include "JNIWrapper.h"
#include "GlobalSingletons/GlobalStore.h"
#include "ImGUIIntegration.h"
#include "JNIUtils.h"
#include "../modules/ModuleHandler.h"
#include "JNIDrefs.h"
#include "JNILogging.h"
#include "JNIModules.h"
#include "JNICommands.h"


int drawWindow(XPLMDrawingPhase     inPhase, int inIsBefore, void *inRefcon) {

	shared_ptr<JNIWrapper> jniInstance = jniutils::getCurrentJNI();

	if (jniInstance->makeWindow && jniInstance->windowExited) {
		jniInstance->windowExited = false;
		jniInstance->window = new JavaXPWindow(640, 480, 1, "Java2XP Window");

		jniInstance->makeWindow = false;
	}
	else {
		jniInstance->makeWindow = false;
	}

	if (jniInstance->window) {
		if (!jniInstance->window->getIsCmdVisible() && !jniInstance->window->isVisible()) {

			jniInstance->windowExitedCB();

		}
	}

	return 1;
}

int showWindow(XPLMCommandRef inCommand, XPLMCommandPhase     inPhase, void *               inRefcon) {

	shared_ptr<JNIWrapper> jniInstance = jniutils::getCurrentJNI();
	jniInstance->makeWindow = true;
	return 0;
}

JNIWrapper::JNIWrapper()
{
	logger = new Logger("JNIWrapper");
	instance = PluginRunner::getGlobalPlugin()->getDrefStore();
	javaInitialized = startJVM();
	if (javaInitialized) {

		initializeClasses();

		jclass mainClass = classes.at("Main");
		jmethodID mainMethod = methods.at("initProject");
		env->CallIntMethod(mainClass, mainMethod, nullptr);

		initializeWindow();

		string message = "JVM State after loading process: \nSuccessfully registered " + to_string(classes.size()) + " classes\nSuccessfully registered " + to_string(methods.size()) + " methods";
		logger->logString(message);

		CallbackInterface::getInstance()->registerCB(std::bind(&JNIWrapper::update, this));
	}
	else {
		logger->logString("Unable to start JVM!\nPossibly restart the simulator to fix the issue!");
	}
}

JNIWrapper::~JNIWrapper()
{
	stop();
}

bool JNIWrapper::startJVM()
{
	JavaVMInitArgs argsInit;

	JavaVMOption* options = new JavaVMOption[2];
	string dllDir = GlobalStore::getInstance()->getProperty("dllDirectory");
	string jarFile = GlobalStore::getInstance()->getProperty("projectJarFile");


	string classPath = dllDir + "projectJar/" + jarFile;

	string optionClassPath = "-Djava.class.path=" + classPath;
	options[0].optionString = (char *)optionClassPath.c_str();
	options[1].optionString = (char *)"-agentlib:jdwp=transport=dt_socket,server=y,suspend=n,address=2000";

	argsInit.version = JNI_VERSION_1_8;
	argsInit.nOptions = 2;
	argsInit.options = options;
	argsInit.ignoreUnrecognized = false;


	jint result = JNI_CreateJavaVM(&javaVM, (void**)&env, &argsInit);


	delete options;

	if (result != JNI_OK) {
		logger->logString("Failed to initialize JVM!\nTrying to attach!");
		return findLoadedJVM(classPath);

	}
	else {
		logger->logString("JVM Started successfully!");
	}



	return true;
}

void JNIWrapper::initializeWindow()
{

	XPLMRegisterDrawCallback(drawWindow, xplm_Phase_Window, 0, NULL);

	logWindowCommand = XPLMCreateCommand("java2xp/showWindow", "Shows the log window for java2xp");

	XPLMRegisterCommandHandler(logWindowCommand, showWindow, 0, NULL);

	ourMenu = XPLMCreateMenu("Java2XP", NULL, NULL, NULL, NULL);
	XPLMAppendMenuItemWithCommand(ourMenu, "Show Log Output", logWindowCommand);

	window = nullptr;
}

void JNIWrapper::update()
{

	if (!drefsInitialized) {

		jclass drefClass = classes.at("org/main/Drefs/DrefNativeInterface");

		jmethodID registerInitialDrefs = methods.at("registerAll");

		env->CallVoidMethod(drefClass, registerInitialDrefs, nullptr);

		drefsInitialized = true;
	}

	jclass updateClass = classes.at("org/main/module/ModuleHandler");

	jmethodID updateMethod = methods.at("updateModules");

	env->CallVoidMethod(updateClass, updateMethod, nullptr);


}

bool JNIWrapper::findLoadedJVM(std::string jarPath)
{

	JavaVM** createdVMS = new JavaVM*[1];
	jsize numVMS;
	env = nullptr;

	JNI_GetCreatedJavaVMs(createdVMS, 1, &numVMS);

	javaVM = createdVMS[0];

	jint result = javaVM->GetEnv((void**)&env, JNI_VERSION_1_8);

	if (result == JNI_EDETACHED) {

		javaVM->AttachCurrentThread((void**)&env, NULL);

		result = javaVM->GetEnv((void**)&env, JNI_VERSION_1_8);

	}

	addSystemClassLoaderPath(jarPath);

	return (result == JNI_OK);

}

void JNIWrapper::addSystemClassLoaderPath(std::string classPath)
{

	// construct URL from file path
	jstring fpStr = env->NewStringUTF(classPath.c_str());

	jclass cls = env->FindClass("java/io/File");

	jmethodID mtdId = env->GetMethodID(cls, "<init>", "(Ljava/lang/String;)V");

	jobject file = env->NewObject(cls, mtdId, fpStr);

	mtdId = env->GetMethodID(cls, "toURI", "()Ljava/net/URI;");

	jobject uri = env->CallObjectMethod(file, mtdId);

	cls = env->FindClass("java/net/URI");

	mtdId = env->GetMethodID(cls, "toURL", "()Ljava/net/URL;");

	jobject url = env->CallObjectMethod(uri, mtdId);

	// get system classloader

	jclass classloaderClass = env->FindClass("java/lang/ClassLoader");

	mtdId = env->GetStaticMethodID(classloaderClass, "getSystemClassLoader", "()Ljava/lang/ClassLoader;");

	jobject sysClsLoader = env->CallStaticObjectMethod(classloaderClass, mtdId);

	// get addURL method 

	jclass urlClassloaderClass = env->FindClass("java/net/URLClassLoader");

	mtdId = env->GetMethodID(urlClassloaderClass, "addURL", "(Ljava/net/URL;)V");

	// add url

	env->CallVoidMethod(sysClsLoader, mtdId, url);

}

void JNIWrapper::stop()
{
	javaVM->DetachCurrentThread();
	javaVM = nullptr;
	env = nullptr;
	/*if (logger) {
		logger->setLogLevel(Logger::log_debug);
		logger->logString("JNI Shutting down!");
	}*/
	XPLMDestroyMenu(ourMenu);
	delete logger;
}



bool JNIWrapper::registerNatives()
{
	logger->logString("Registering Natives!");
	JNINativeMethod methods[]{
		{(char *)"registerDref",(char *)"(Ljava/lang/String;I)V",(void *)JNIDrefFunctions::registerDref},
		{(char*)"getDrefInt",(char *)"(Ljava/lang/String;)I",(void *)JNIDrefFunctions::getIntRef},
		{(char*)"getDrefFloat",(char *)"(Ljava/lang/String;)F",(void*)JNIDrefFunctions::getFloatRef},
		{(char*)"getDrefString",(char *)"(Ljava/lang/String;)Ljava/lang/String;",(void *)JNIDrefFunctions::getStringRef},
		{(char*)"getDrefFloatArr",(char *)"(Ljava/lang/String;)[F",(void *)JNIDrefFunctions::getFloatArrayRef},
		{(char *)"getDrefIntArr",(char *) "(Ljava/lang/String;)[I",(void *)JNIDrefFunctions::getIntArrayRef},
		{(char *)"setDrefInt",(char *)"(Ljava/lang/String;I)V",(void*)JNIDrefFunctions::setIntRef},
		{(char *)"setDrefFloat",(char *)"(Ljava/lang/String;F)V",(void*)JNIDrefFunctions::setFloatRef},
		{(char*)"setDrefString",(char *)"(Ljava/lang/String;Ljava/lang/String;)V",(void *)JNIDrefFunctions::setStringRef}
	};

	JNINativeMethod loggers[]{
		{(char *)"logToWindow",(char *)"(Ljava/lang/String;)V",(void *)JNILogging::appendChar}
	};

	JNINativeMethod utils[]{

		{(char*)"getProjectDirectory",(char*)"()Ljava/lang/String;",(void *)jniutils::getProjectDirectory},
		{(char*)"getAircraftDirectory",(char*)"()Ljava/lang/String;",(void *)jniutils::getAircraftDirectory},
		{(char*)"getSimDirectory",(char*)"()Ljava/lang/String;",(void *)jniutils::getXPlaneDirectory},
	};

	JNINativeMethod modules[]{
		{(char*)"setNativeGraphicsModuleID",(char*)"(I)V",(void *)JNIModules::setGraphicsModule},
		{(char*)"createNewNativeModule",(char*)"(Ljava/lang/String;)I",(void*)JNIModules::createNewModule},
		{(char*)"createNewNativeGraphicsModule",(char*)"(Ljava/lang/String;IIII)I",(void*)JNIModules::createNewGraphicsModule}
	};

	JNINativeMethod initEverything[]{
		{(char*)"enableGraphicsBackend",(char*)"(II)V",(void*)JNIModules::enableGraphicsBackend}
	};

	JNINativeMethod commandMethods[]
	{
		{(char*)"findCommandNative",(char*)"(Ljava/lang/String;)I",(void*)JNICommands::findCommand},
		{(char*)"createCommandNative",(char*)"(Ljava/lang/String;Ljava/lang/String;)I",(void*)JNICommands::createCommand},
		{(char*)"registerCommandHandlerNative",(char*)"(II)I",(void*)JNICommands::registerCommandHandler},
		{(char*)"unregisterCommandHandlerNative",(char*)"(III)V",(void*)JNICommands::unregisterCommandHandler}
		
	};

	const jclass drefNativeInterface = classes.at("org/main/Drefs/DrefNativeInterface");

	jint result = env->RegisterNatives(drefNativeInterface, methods, 9);

	if (result < 0) {
		logger->logString("Failed to register natives for Dataref Interface");
		return false;
	}

	const jclass loggerOutput = classes.at("org/main/UpdateLoop/LogOutput");

	result = env->RegisterNatives(loggerOutput, loggers, 1);

	if (result < 0) {
		logger->logString("Failed to register natives for FLC");
		return false;
	}

	const jclass utilClass = classes.at("org/main/utility/Utility");

	result = env->RegisterNatives(utilClass, utils, 3);

	if (result < 0) {
		logger->logString("Failed to register natives for Utility class!");
		return false;
	}

	const jclass moduleHandlerClass = classes.at("org/main/module/ModuleHandler");

	result = env->RegisterNatives(moduleHandlerClass, modules, 3);

	if (result < 0) {
		logger->logString("Failed to register natives for ModuleHandler class!");
		return false;
	}

	const jclass initEverythingClass = classes.at("org/main/InitEverything");

	result = env->RegisterNatives(initEverythingClass, initEverything, 1);

	if (result < 0) {
		logger->logString("Failed to register natives for InitEverything class!");
		return false;
	}

	const jclass commandInteface = classes.at("org/main/commands/CommandInterface");
	result = env->RegisterNatives(commandInteface, commandMethods, 4);

	if(result < 0)
	{
		logger->logString("Failed to register natives for CommandInterface class!");
		return false;
	}

	return true;
}



void JNIWrapper::findAndAddMethod(string methodName, string className, string signature)
{
	jclass methodClass;
	try {
		methodClass = classes.at(className);
	}
	catch (std::out_of_range ex) {
		logger->setLogLevel(Logger::log_error);
		logger->logString("Class " + className + " for method " + methodName + " not found!");
		return;
	}
	jmethodID methodID = env->GetMethodID(methodClass, methodName.c_str(), signature.c_str());
	methods.insert(std::pair<string, jmethodID>(methodName, methodID));
}

void JNIWrapper::findAndAddStaticMethod(string methodName, string className, string signature)
{
	jclass methodClass;
	try {
		methodClass = classes.at(className);
	}
	catch (std::out_of_range ex) {
		logger->setLogLevel(Logger::log_error);
		logger->logString("Class " + className + " for static method " + methodName + " not found!");
		return;
	}
	jmethodID methodID = env->GetStaticMethodID(methodClass, methodName.c_str(), signature.c_str());
	methods.insert(std::pair<string, jmethodID>(methodName, methodID));
}

void JNIWrapper::findAndLoadClass(string className)
{
	jclass javaClass = env->FindClass(className.c_str());

	if (!javaClass) {
		logger->setLogLevel(Logger::log_error);
		logger->logString("Class : " + className + " was not found by the JVM!");
		return;
	}

	classes.insert(std::pair<string, jclass>(className, javaClass));
}

void JNIWrapper::initializeClasses()
{
	findAndLoadClass("org/main/InitEverything");
	findAndLoadClass("org/main/Drefs/DrefNativeInterface");
	findAndLoadClass("org/main/UpdateLoop/LogOutput");
	findAndLoadClass("org/main/UpdateLoop/FlightLoopCB");
	findAndLoadClass("org/main/InitEverything");
	findAndLoadClass("org/main/utility/Utility");
	findAndLoadClass("Main");
	findAndLoadClass("org/main/graphics/PrimitiveGraphics");
	findAndLoadClass("org/main/graphics/Font");
	findAndLoadClass("org/main/graphics/Color");
	findAndLoadClass("org/main/module/ModuleHandler");
	findAndLoadClass("org/main/commands/CommandInterface");

	findAndAddStaticMethod("initProject", "Main", "([Ljava/lang/String;)I");
	findAndAddStaticMethod("exit", "org/main/InitEverything", "()V");
	findAndAddStaticMethod("update", "org/main/UpdateLoop/FlightLoopCB", "()V");
	findAndAddStaticMethod("registerAll", "org/main/Drefs/DrefNativeInterface", "()V");
	findAndAddStaticMethod("updateModules", "org/main/module/ModuleHandler", "()V");
	findAndAddStaticMethod("drawModules", "org/main/module/ModuleHandler", "()V");
	findAndAddStaticMethod("executeCommandHandler", "org/main/commands/CommandInterface", "(II)I");


	registerNatives();


}

int JNIWrapper::callCommandCallback(int callbackID, int phaseID)
{
	jclass commandInterface = classes.at("org/main/commands/CommandInterface");
	jmethodID commandHandlerMethod = methods.at("executeCommandHandler");

	return env->CallStaticIntMethod(commandInterface, commandHandlerMethod, callbackID, phaseID);
}

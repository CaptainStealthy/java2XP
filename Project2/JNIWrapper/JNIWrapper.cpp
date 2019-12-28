#include "JNIWrapper.h"
#include "GlobalSingletons/GlobalStore.h"
#include "ImGUIIntegration.h"
#include "JNIUtils.h"


void appendChar(JNIEnv *e, jobject o, jstring character);
void registerDref(JNIEnv *e, jobject o, jstring name, jint type);
jfloat getFloatRef(JNIEnv* e, jobject o, jstring name);
jint getIntRef(JNIEnv*e, jobject o, jstring name);
jstring getStringRef(JNIEnv *e, jobject o, jstring name);
jfloatArray getFloatArrayRef(JNIEnv *e, jobject o, jstring name);
jintArray getIntArrayRef(JNIEnv *e, jobject o, jstring name);
void setFloatRef(JNIEnv*e, jobject o, jstring name, jfloat value);
void setIntRef(JNIEnv*e, jobject o, jstring name, jint value);
void setStringRef(JNIEnv*e, jobject o, jstring name, jstring value);
void setFloatArrayRef(JNIEnv * e, jobject o, jstring name, jobjectArray values);
void setIntArrayRef(JNIEnv * e, jobject o, jstring name, jobjectArray values);
void appendChar(JNIEnv * e, jobject o, jstring character);

jstring getProjectDirectory(JNIEnv * e, jobject o);
jstring getXPlaneDirectory(JNIEnv * e, jobject o);
jstring getAircraftDirectory(JNIEnv * e, jobject o);

std::shared_ptr<JNIWrapper> getCurrentJNI();



int drawWindow(XPLMDrawingPhase     inPhase,int inIsBefore,void *inRefcon) {
	
	shared_ptr<JNIWrapper> jniInstance = getCurrentJNI();

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

int  showWindow(XPLMCommandRef       inCommand, XPLMCommandPhase     inPhase, void *               inRefcon) {

	shared_ptr<JNIWrapper> jniInstance = getCurrentJNI();
	jniInstance->makeWindow = true;
	return 0;
}

JNIWrapper::JNIWrapper()
{
	logger = new Logger("JNIWrapper");
	instance = DrefStore::getInstance();
	javaInitialized  = startJVM();
	if (javaInitialized) {
		findAndLoadClass("org/main/Drefs/DrefNativeInterface");
		findAndLoadClass("org/main/UpdateLoop/LogOutput");
		findAndLoadClass("org/main/UpdateLoop/FlightLoopCB");
		findAndLoadClass("org/main/InitEverything");
		findAndLoadClass("org/main/utility/Utility");
		findAndLoadClass("Main");
		registerNatives();
		findAndAddStaticMethod("initProject", "Main", "([Ljava/lang/String;)I");
		findAndAddStaticMethod("exit", "org/main/InitEverything", "()V");
		findAndAddStaticMethod("update", "org/main/UpdateLoop/FlightLoopCB", "()V");
		findAndAddStaticMethod("registerAll", "org/main/Drefs/DrefNativeInterface", "()V");


		jclass mainClass = classes.at("Main");
		jmethodID mainMethod = methods.at("initProject");
		env->CallIntMethod(mainClass, mainMethod, nullptr);

		XPLMRegisterDrawCallback(drawWindow, xplm_Phase_Window, 0, NULL);

		logWindowCommand = XPLMCreateCommand("java2xp/showWindow", "Shows the log window for java2xp");

		XPLMRegisterCommandHandler(logWindowCommand, showWindow, 0, NULL);

		ourMenu = XPLMCreateMenu("Java2XP", NULL, NULL, NULL, NULL);
		XPLMAppendMenuItemWithCommand(ourMenu, "Show Log Output", logWindowCommand);

		string message = "JVM State after loading process: \nSuccessfully registered " + to_string(classes.size()) + " classes\nSuccessfully registered " + to_string(methods.size()) + " methods";
		logger->logString(message);

		window = nullptr;

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
	options[0].optionString =(char *) optionClassPath.c_str();
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

void JNIWrapper::update()
{
	if (jvmInitialized) {

		if (!drefsInitialized) {

			jclass drefClass = classes.at("org/main/Drefs/DrefNativeInterface");

			jmethodID registerInitialDrefs = methods.at("registerAll");

			env->CallVoidMethod(drefClass, registerInitialDrefs, nullptr);

			drefsInitialized = true;
		}

		jclass updateClass = classes.at("org/main/UpdateLoop/FlightLoopCB");

		jmethodID updateMethod = methods.at("update");

		env->CallVoidMethod(updateClass, updateMethod, nullptr);

	}

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
	jclass initClass = classes.at("org/main/InitEverything");
	jmethodID exitMethod = methods.at("exit");
	//env->CallVoidMethod(initClass, exitMethod);
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
		{(char *)"registerDref",(char *)"(Ljava/lang/String;I)V",(void *)registerDref},
		{(char*)"getDrefInt",(char *)"(Ljava/lang/String;)I",(void *)getIntRef},
		{(char*)"getDrefFloat",(char *)"(Ljava/lang/String;)F",(void*)getFloatRef},
		{(char*)"getDrefString",(char *)"(Ljava/lang/String;)Ljava/lang/String;",(void *)getStringRef},
		{(char*)"getDrefFloatArr",(char *)"(Ljava/lang/String;)[F",(void *)getFloatArrayRef},
		{(char *)"getDrefIntArr",(char *) "(Ljava/lang/String;)[I",(void *)getIntArrayRef},
		{(char *)"setDrefInt",(char *)"(Ljava/lang/String;I)V",(void*)setIntRef},
		{(char *)"setDrefFloat",(char *)"(Ljava/lang/String;F)V",(void*)setFloatRef},
		{(char*)"setDrefString",(char *)"(Ljava/lang/String;Ljava/lang/String;)V",(void *)setStringRef}
	};

	jclass drefNativeInterface = classes.at("org/main/Drefs/DrefNativeInterface");

	JNINativeMethod loggers[]{
		{(char *)"logToWindow",(char *)"(Ljava/lang/String;)V",(void *)appendChar}
	};

	JNINativeMethod utils[]{

		{(char*)"getProjectDirectory",(char*)"()Ljava/lang/String;",(void *)getProjectDirectory},
		{(char*)"getAircraftDirectory",(char*)"()Ljava/lang/String;",(void *)getAircraftDirectory},
		{(char*)"getSimDirectory",(char*)"()Ljava/lang/String;",(void *)getXPlaneDirectory}
	};

	jint result = env->RegisterNatives(drefNativeInterface, methods, 9);

	if (result < 0) {
		logger->logString("Failed to register natives for Dataref Interface");
		return false;
	}

	jclass loggerOutput = classes.at("org/main/UpdateLoop/LogOutput");

	result = env->RegisterNatives(loggerOutput, loggers, 1);

	if (result < 0) {
		logger->logString("Failed to register natives for FLC");
		return false;
	}

	jclass utilClass = classes.at("org/main/utility/Utility");

	result = env->RegisterNatives(utilClass, utils, 3);

	if (result < 0) {
		logger->logString("Failed to register natives for Utility class!");
		return false;
	}

	return true;
}

void registerDref(JNIEnv *e, jobject o,jstring name, jint type)
{

	

	string drefName = e->GetStringUTFChars(name, nullptr);
	int drefType = type;

	

	string drefTypeName;

	switch (drefType) {
		case 0 :
			drefTypeName = "integer";
			break;

		case 1:
			drefTypeName = "decimal";
			break;

		case 2:
			drefTypeName = "string";
			break;

		case 3:
			drefTypeName = "decimal_array";
			break;

		case 4:
			drefTypeName = "int_array";
			break;
	}

	if (!getCurrentJNI() || !getCurrentJNI()->instance) {
		return;
	}

	getCurrentJNI()->instance->add(drefTypeName, drefName, drefName);
}

void JNIWrapper::findAndAddMethod(string methodName, string className, string signature)
{
	jclass methodClass;
	try {
		methodClass = classes.at(className);
	}
	catch (std::out_of_range ex) {
		logger->setLogLevel(Logger::log_error);
		logger->logString("Class "+className+" for method "+methodName+" not found!");
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

jfloat getFloatRef(JNIEnv * e, jobject o, jstring name)
{
	string drefName = e->GetStringUTFChars(name, nullptr);
	float value = getCurrentJNI()->instance->getDecimal(drefName);

	return (jfloat)value;
}

jint getIntRef(JNIEnv * e, jobject o, jstring name)
{
	string drefName = e->GetStringUTFChars(name, nullptr);
	int value = getCurrentJNI()->instance->getInt(drefName);

	return (jint)value;
}

jstring getStringRef(JNIEnv * e, jobject o, jstring name)
{
	string drefName = e->GetStringUTFChars(name, nullptr);
	string value = getCurrentJNI()->instance->getString(drefName);

	jstring result = e->NewStringUTF(value.c_str());

	return result;
}

jfloatArray getFloatArrayRef(JNIEnv * e, jobject o, jstring name)
{
	string drefName = e->GetStringUTFChars(name, nullptr);
	shared_ptr<dref> drefToGet = getCurrentJNI()->instance->getDrefReference(drefName);

	int lenOfArray = drefToGet->getArraySize();

	jfloatArray arr = e->NewFloatArray(lenOfArray);

	double* arrayValues = getCurrentJNI()->instance->getDecimalArray(drefName);

	for (int i = 0; i < lenOfArray; i++) {
		jfloat value = (jfloat)arrayValues[i];
		e->SetFloatArrayRegion(arr, i, 1, &value);
	}

	return arr;
}

jintArray getIntArrayRef(JNIEnv * e, jobject o, jstring name)
{

	string drefName = e->GetStringUTFChars(name, nullptr);
	shared_ptr<dref> drefToGet = getCurrentJNI()->instance->getDrefReference(drefName);

	int lenOfArray = drefToGet->getArraySize();

	jintArray arr = e->NewIntArray(lenOfArray);

	int* arrayValues = drefToGet->getAsIntArray();

	for (int i = 0; i < lenOfArray; i++) {
		jint value = (jint)arrayValues[i];
		e->SetIntArrayRegion(arr, i, 1, &value);
	}

	return arr;
}

void setFloatRef(JNIEnv * e, jobject o, jstring name, jfloat value)
{
	string drefName = e->GetStringUTFChars(name, nullptr);

	float valueToSet = value;

	getCurrentJNI()->instance->setDecimal(drefName, valueToSet);

}

void setIntRef(JNIEnv * e, jobject o, jstring name, jint value)
{
	string drefName = e->GetStringUTFChars(name, nullptr);
	int valueToSet = value;

	getCurrentJNI()->instance->setInt(drefName, valueToSet);

}

void  setStringRef(JNIEnv * e, jobject o, jstring name, jstring value)
{
	string drefName = e->GetStringUTFChars(name, nullptr);
	string valueToSet = e->GetStringUTFChars(value, nullptr);

	getCurrentJNI()->instance->setString(drefName, valueToSet);
}

void  setFloatArrayRef(JNIEnv * e, jobject o, jstring name, jobjectArray values)
{
}

void  setIntArrayRef(JNIEnv * e, jobject o, jstring name, jobjectArray values)
{
}


void appendChar(JNIEnv * e, jobject o, jstring character)
{
	string value = e->GetStringUTFChars(character, nullptr);
	if (getCurrentJNI() && getCurrentJNI()->window) {
		getCurrentJNI()->window->appendCharacterToWindow(value);
	}
	XPLMDebugString(value.c_str());
}


std::shared_ptr<JNIWrapper> getCurrentJNI() {
	std::shared_ptr<PluginRunner> plugin = PluginRunner::getCurrentPlugin();
	return plugin->getJNI();
}

jstring getProjectDirectory(JNIEnv * e, jobject o) {
	jstring projectPath = e->NewStringUTF(jniutils::getProjectPath().c_str());
	return projectPath;
}

jstring getXPlaneDirectory(JNIEnv * e, jobject o) {
	jstring xplanePath = e->NewStringUTF(jniutils::getXplanePath().c_str());
	return xplanePath;
}

jstring getAircraftDirectory(JNIEnv * e, jobject o) {
	jstring aircraftPath = e->NewStringUTF(jniutils::getAircraftPath().c_str());
	return aircraftPath;
}
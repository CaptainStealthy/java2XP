#include "JNIUtils.h"

string jniutils::getXplanePath()
{

	char buf[1024];
	XPLMGetSystemPath(buf);
	string path = buf;
	return path;
	
}

string jniutils::getProjectPath()
{
	GlobalStore* instance = GlobalStore::getInstance();
	return instance->getProperty("dllDirectory") + "projectJar/";
}

string jniutils::getAircraftPath()
{
	string projectPath = getProjectPath();
	string aircraftPath = projectPath.substr(0, projectPath.find_last_of("\\"));
	aircraftPath = aircraftPath.substr(0, aircraftPath.find_last_of("\\"));
	aircraftPath = aircraftPath.substr(0, aircraftPath.find_last_of("\\"));
	aircraftPath = aircraftPath.substr(0, aircraftPath.find_last_of("\\")) + "\\";

	return aircraftPath;
}

jstring jniutils::getProjectDirectory(JNIEnv* e, jobject o)
{
	jstring projectPath = e->NewStringUTF(jniutils::getProjectPath().c_str());
	return projectPath;
}

std::shared_ptr<JNIWrapper> jniutils::getCurrentJNI()
{
	return PluginRunner::getGlobalPlugin()->getJNI();
}

jstring jniutils::getXPlaneDirectory(JNIEnv* e, jobject o)
{

	jstring xplanePath = e->NewStringUTF(jniutils::getXplanePath().c_str());
	return xplanePath;
	
}

jstring jniutils::getAircraftDirectory(JNIEnv* e, jobject o)
{
	jstring aircraftPath = e->NewStringUTF(jniutils::getAircraftPath().c_str());
	return aircraftPath;
}

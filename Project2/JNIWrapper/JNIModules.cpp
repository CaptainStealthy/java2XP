#include "JNIModules.h"


jint JNIModules::createNewModule(JNIEnv* e, jobject o, jstring name)
{

	std::string value = e->GetStringUTFChars(name, nullptr);
	std::shared_ptr<PluginRunner> plugin = PluginRunner::getGlobalPlugin();
	std::shared_ptr<ModuleHandler> moduleHandler = plugin->getModuleHandler();
	int moduleID = moduleHandler->createNewModule(value);
	return (jint)moduleID;
	
}

jint JNIModules::createNewGraphicsModule(JNIEnv* e, jobject o, jstring name, jint x, jint y, jint height, jint width)
{
	string value = e->GetStringUTFChars(name, nullptr);
	std::shared_ptr<PluginRunner> plugin = PluginRunner::getGlobalPlugin();
	std::shared_ptr<ModuleHandler> moduleHandler = plugin->getModuleHandler();
	int moduleID = moduleHandler->createNewGraphicsModule(value, x, y, height, width);
	return (jint)moduleID;
	
}

void JNIModules::setGraphicsModule(JNIEnv* e, jobject o, jint moduleID)
{
	std::shared_ptr<PluginRunner> plugin = PluginRunner::getGlobalPlugin();
	std::shared_ptr<ModuleHandler> moduleHandler = plugin->getModuleHandler();
	moduleHandler->setCurrentRenderingModule(moduleID);
}

void JNIModules::enableGraphicsBackend(JNIEnv* e, jobject o, jint x, jint y)
{
	GlobalStore::getInstance()->addProperty("enableCockpitDrawing", "true");
	GlobalStore::getInstance()->addProperty("cockpitSurfaceX", to_string(x));
	GlobalStore::getInstance()->addProperty("cockpitSurfaceY", to_string(y));
}

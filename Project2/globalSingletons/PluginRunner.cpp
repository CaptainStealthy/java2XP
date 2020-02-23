#include "PluginRunner.h"
#include <utility/Logger.h>
#include <string>
#include <XPLMProcessing.h>
#include <XPLMDisplay.h>
#include <Datarefs/drefStore.h>
#include <globalSingletons/GlobalStore.h>

#include <CallbackInterface.h>
#include "JNIWrapper/JNIWrapper.h"
#include "graphics/mainRenderer/CockpitRenderer.h"
using namespace std;


shared_ptr<PluginRunner> PluginRunner::myInstance = nullptr;



void PluginRunner::update()
{
	
}


void PluginRunner::cleanup(void)
{
	
	jniWrapper = nullptr;
	CallbackInterface::getInstance()->stop();
	
	//CockpitRenderer::getRendererInstance()->destroyCockpitRenderer();
	myInstance = nullptr;
}


void PluginRunner::initializePlugin()
{

	CallbackInterface::makeNewInterface();

	moduleHandler = std::make_shared<ModuleHandler>();

	logger = make_unique<Logger>("Plugin Runner");
	logger->setLogLevel(Logger::log_message);

	string message = "Loading Magknight 787-9 Aviators Edition " + GlobalStore::getInstance()->getProperty("version");
	logger->logString(message);

	logger->logString("PluginRunner initializing everything");
	drefStore = std::make_shared<DrefStore>();
	CallbackInterface::getInstance()->registerCB(std::bind(&PluginRunner::update, this));
	

	logger->setLogLevel(Logger::log_message);
	logger->logString("PluginRunner Started");

	commandInterface = make_shared<CommandInterface>();

	jniWrapper = make_shared<JNIWrapper>();

	
	

	

	graphicsBackendEnabled = (GlobalStore::getInstance()->getProperty("enableCockpitDrawing") == "true") ? true : false;

	//CockpitRenderer::createCockpitRenderer(4096,4096);

}

PluginRunner::PluginRunner()
{

}



PluginRunner::~PluginRunner()
{
}

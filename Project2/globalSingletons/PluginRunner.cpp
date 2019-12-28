#include "PluginRunner.h"
#include <utility/Logger.h>
#include <string>
#include <XPLMProcessing.h>
#include <XPLMDisplay.h>
#include <globalSingletons/drefStore.h>
#include <globalSingletons/GlobalStore.h>
#include<globalSingletons/graphicsWrapper.h>

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
	drefStore->stop();
	CallbackInterface::getInstance()->stop();
	
	CockpitRenderer::getRendererInstance()->destroyCockpitRenderer();
	myInstance = nullptr;
}



void PluginRunner::draw2d()
{
}

void PluginRunner::draw3d()
{
}

void PluginRunner::initializePlugin()
{

	CallbackInterface::makeNewInterface();


	logger = make_unique<Logger>("Plugin Runner");
	logger->setLogLevel(Logger::log_message);

	string message = "Loading Magknight 787-9 Aviators Edition " + GlobalStore::getInstance()->getProperty("version");
	logger->logString(message);

	logger->logString("PluginRunner initializing everything");
	drefStore = DrefStore::createNewStore();
	CallbackInterface::getInstance()->registerCB(std::bind(&PluginRunner::update, this));
	jniWrapper = make_shared<JNIWrapper>();

	logger->setLogLevel(Logger::log_message);
	logger->logString("PluginRunner Started");

	jniWrapper->setInitialized(true);


	CockpitRenderer::createCockpitRenderer(4096,4096);

}

PluginRunner::PluginRunner()
{

}



PluginRunner::~PluginRunner()
{
	cout << "It works!" << endl;
}

#include "PluginRunner.h"
#include <utility/Logger.h>
#include <string>
#include <XPLMProcessing.h>
#include <XPLMDisplay.h>
#include <globalSingletons/drefStore.h>
#include <globalSingletons/GlobalStore.h>
#include<globalSingletons/graphicsWrapper.h>
#include<globalSingletons/RainEffects.h>
#include <CallbackInterface.h>
#include "JNIWrapper/JNIWrapper.h"
using namespace std;


//THIS CLASS IS SUPPOSED TO RUN AND MANAGE ALL OF THE PLUGIN'S FUNCTIONS
//WE NEED A SINGLETON INSTANCE OF EACH SINGLETON CLASS

shared_ptr<PluginRunner> PluginRunner::myInstance = nullptr;



void PluginRunner::update()
{
	
}


void PluginRunner::cleanup(void)
{
	
	jniWrapper = nullptr;
	drefStore->stop();
	myInstance = nullptr;
	CallbackInterface::getInstance()->stop();
}



void PluginRunner::draw2d()
{
}

void PluginRunner::draw3d()
{
}

PluginRunner::PluginRunner()
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
}



PluginRunner::~PluginRunner()
{
	cout << "It works!" << endl;
}

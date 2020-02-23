#include "ModuleHandler.h"



ModuleHandler::ModuleHandler()
{
}


ModuleHandler::~ModuleHandler()
{
}

int ModuleHandler::getNextModuleID()
{
	return allModules.size() + 1;
}

int ModuleHandler::createNewModule(std::string moduleName)
{
	int newModuleID = getNextModuleID();
	std::shared_ptr<Module> newModule = std::make_shared<Module>(moduleName, newModuleID);
	allModules.insert(std::make_pair(newModuleID, newModule));
	return newModuleID;
}

int ModuleHandler::createNewGraphicsModule(std::string moduleName, int x, int y, int width, int height)
{
	int newModuleID = getNextModuleID();
	std::shared_ptr<CockpitModule> newModule = std::make_shared<CockpitModule>(moduleName, newModuleID, x, y, width, height);
	std::shared_ptr<Module> moduleDowncast = std::dynamic_pointer_cast<Module>(newModule);
	allModules.insert(std::make_pair(newModuleID, moduleDowncast));
	return newModuleID;

}

void ModuleHandler::setCurrentRenderingModule(int moduleID)
{
	
	if (allModules.find(moduleID) == allModules.end()) {
		throw new std::exception("Invalid module ID passed to render to.");
		logger.setLogLevel(Logger::log_error);
		logger.logString("Invalid module ID: " + std::to_string(moduleID) + " passed to render to.");
	}
	else {
		this->renderingModule = std::static_pointer_cast<CockpitModule>(allModules.at(moduleID));
	}

}

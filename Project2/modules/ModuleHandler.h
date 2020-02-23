#pragma once

#include <map>
#include "Module.h"
#include "../graphics/module/CockpitModule.h"
#include <memory>
#include "../utility/Logger.h"

class ModuleHandler
{
public:
	ModuleHandler();
	~ModuleHandler();

	int getNextModuleID();

	int createNewModule(std::string moduleName);
	int createNewGraphicsModule(std::string moduleName, int x, int y, int width, int height);
	void setCurrentRenderingModule(int moduleID);

private:

	std::map<int, std::shared_ptr<Module>> allModules;
	std::shared_ptr<CockpitModule> renderingModule;
	Logger logger = Logger("Module Handler");


};


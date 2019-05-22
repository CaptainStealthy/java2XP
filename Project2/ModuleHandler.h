#pragma once

#include <vector>
#include <string>
#include <Module.h>
#include <map>

class ModuleHandler
{
public:
	static std::map<int, Module*> allModules;

	static void addModule(Module* module) {
		allModules.insert(std::make_pair(allModules.size()+1,module));
		module->setModuleID(allModules.size());
	}

	static Module* getModuleByID(int ID) {
		return allModules.at(ID);
	}

	static void drawAllModules() {
			
		for (auto modules : allModules) {
			modules.second->draw();
		}
	}

	static void updateAllModules() {
		for (auto modules : allModules) {
			modules.second->update();
		}
	}

	ModuleHandler();
	~ModuleHandler();
};


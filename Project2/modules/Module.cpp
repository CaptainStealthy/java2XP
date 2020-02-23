#include "Module.h"


Module::Module(std::string moduleName, int moduleID)
{
	this->moduleID = moduleID;
	this->moduleName = moduleName;
}

Module::~Module()
{
}

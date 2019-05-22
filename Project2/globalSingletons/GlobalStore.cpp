#include "GlobalStore.h"

static std::map<std::string, std::string> globalStore;

GlobalStore* GlobalStore::singleInstance = nullptr;

GlobalStore::GlobalStore()
{
}

void GlobalStore::addProperty(std::string property, std::string value)
{
	globalStore[property] = value;
}

void GlobalStore::setProperty(std::string property, std::string value)
{
	globalStore[property] = value;
}

std::string GlobalStore::getProperty(std::string property)
{
	return globalStore[property];
}


GlobalStore::~GlobalStore()
{
}

#pragma once
#include <utility/Logger.h>
#include <Datarefs/drefStore.h>
#include "JNIWrapper/JNIWrapper.h"
#include "RainEffects.h"
#include "../modules/ModuleHandler.h"
#include "../commands/CommandInterface.h"

class JNIWrapper;
class CommandInterface;

class PluginRunner {

public:
	

	void cleanup(void);

	void update();


	std::shared_ptr<JNIWrapper> getJNI() {
		return jniWrapper;
	}

	std::shared_ptr<DrefStore> getDrefStore() {
		return this->drefStore;
	}

	void setCurrentPlugin(std::shared_ptr<PluginRunner> myInstance) {
		this->myInstance = myInstance;
	}

	static std::shared_ptr<PluginRunner> getGlobalPlugin() {
		return myInstance;
	}

	std::shared_ptr<ModuleHandler> getModuleHandler() {
		return this->moduleHandler;
	}

	std::shared_ptr<CommandInterface> getCommandInterface()
	{
		return this->commandInterface;
	}

	void initializePlugin();

	PluginRunner();
	~PluginRunner();

private:
	std::unique_ptr<Logger> logger;
	std::shared_ptr<DrefStore> drefStore;
	std::shared_ptr<JNIWrapper> jniWrapper;
	std::shared_ptr<ModuleHandler> moduleHandler;
	std::shared_ptr<CommandInterface> commandInterface;

	static std::shared_ptr<PluginRunner> myInstance;

	bool graphicsBackendEnabled = false;;


};
#pragma once
#include <utility/Logger.h>
#include <globalSingletons/drefStore.h>
#include "JNIWrapper/JNIWrapper.h"

class JNIWrapper;

class PluginRunner {

public:
	

	void cleanup(void);

	void update();

	void draw2d();

	void draw3d();

	std::shared_ptr<JNIWrapper> getJNI() {
		return jniWrapper;
	}

	void setCurrentPlugin(std::shared_ptr<PluginRunner> myInstance) {
		this->myInstance = myInstance;
	}

	static std::shared_ptr<PluginRunner> getCurrentPlugin() {
		return myInstance;
	}
	PluginRunner();
	~PluginRunner();

private:
	std::unique_ptr<Logger> logger;
	std::shared_ptr<DrefStore> drefStore;
	std::shared_ptr<JNIWrapper> jniWrapper;
	static std::shared_ptr<PluginRunner> myInstance;


};
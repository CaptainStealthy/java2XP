#pragma once
#include <XPLMUtilities.h>
#include <string>
#include "globalSingletons/GlobalStore.h"

using namespace std;

namespace jniutils {

	string getXplanePath() {
		char buf[1024];
		XPLMGetSystemPath(buf);
		string path = buf;
		return path;
	}

	string getProjectPath() {
		GlobalStore* instance = GlobalStore::getInstance();
		return instance->getProperty("dllDirectory") + "projectJar/";
	}

	string getAircraftPath() {
		string projectPath = getProjectPath();
		string aircraftPath = projectPath.substr(0, projectPath.find_last_of("\\"));
		aircraftPath = aircraftPath.substr(0, aircraftPath.find_last_of("\\"));
		aircraftPath = aircraftPath.substr(0, aircraftPath.find_last_of("\\"));
		aircraftPath = aircraftPath.substr(0, aircraftPath.find_last_of("\\"))+"\\";

		return aircraftPath;
	}

}

#pragma comment(lib, "ws2_32.lib")
#define WIN32_LEAN_AND_MEAN

#pragma once

#include <iostream>
#include <fstream>
#include<sstream>


#include <string.h>

#include <thread>
#include<stddef.h>

#if defined(__WIN32__) || defined(_WIN32)
#elif defined(__APPLE__)
#include <unistd.h>
#endif
#include<cstring>
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_image.h>
#include <XPLMDataAccess.h>
#include <process.h>
#include "dirent.h"
#include <chrono>
#include <globalSingletons/PluginRunner.h>
#include <globalSingletons/GlobalStore.h>
#include<utility/Logger.h>
#include<XPLMProcessing.h>
#include<XPLMUtilities.h>
#include <startupChecker/startupChecker.h>
#include "CallbackInterface.h"



using namespace std;
#ifndef XPLM300
#error This is made to be compiled against the XPLM300 SDK
#endif

bool fileScanning = false;

class PluginRunner;

//SINGLETON CLASSES
std::shared_ptr<PluginRunner> pluginRunner;
GlobalStore *gStore = GlobalStore::getInstance();
string dllDirectory;


void loadConfigFile();


/////DREFS FOR SEARCHING FILES
XPLM_API XPLMDataRef directorySearch;
XPLM_API XPLMDataRef filesResult;
XPLM_API XPLMDataRef modulePath;

XPLM_API XPLMDataRef dirSearch;


void readDirectory() {
	try {
		
		XPLMDataRef filesScanned; filesScanned = XPLMFindDataRef("mag787/efb/docs/files/scanned");
		XPLMDataRef fileAdd; fileAdd = XPLMFindDataRef("mag787/efb/docs/fileToAdd");
		XPLMDataRef fileCommand; fileCommand = XPLMFindDataRef("mag787/efb/docs/files/command");

	vector<string> filesInDir;
	dirSearch = XPLMFindDataRef("mag787/efb/docs/searchDir");

	char directoryPath[1024];

	DIR *directory;
	struct dirent *ent;

	int dize = XPLMGetDatab(dirSearch, NULL, 0, 0);

	directoryPath[dize] = '\0';

	XPLMGetDatab(dirSearch, &directoryPath, 0, dize);
	//XPLMDebugString(directoryPath); XPLMDebugString("\n");
	

	directory = opendir(directoryPath);

	if (ENOENT == errno) {
		//SET COMMAND TO 2 SO LUA IS INFORMED ABOUT THE ERROR MESSAGE AS WELL
		XPLMSetDatai(fileCommand, 2);
		
		closedir(directory);
		fileScanning = false;
		return;
	}

	while ((ent = readdir(directory)) != NULL) {
		if (string(ent->d_name).find("pdf") != string::npos) {
			XPLMSetDatab(fileAdd, (void *)ent->d_name, 0, ent->d_namlen);
			XPLMSetDatai(fileCommand, 1);
			int returned = XPLMGetDatai(fileCommand);
			
			while ((returned = XPLMGetDatai(fileCommand)) == 1)
			{
				//WAIT FOR LUA TO ACTUALLY ADD THE FILE
			}

		}
	}
	XPLMSetDatai(filesScanned, 1);
	closedir(directory);
	fileScanning = false;
}
catch (exception e) {
	
}
}

string getModuleDirectory() {
	string dir;
	char path[MAX_PATH];
	HMODULE hm = NULL;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
		GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)&readDirectory, &hm);
	GetModuleFileName(hm, path, sizeof(path));

	dir = path;
	dir = dir.substr(0, dir.find_last_of("\\")) + "\\";
	dllDirectory = dir;

	return dir;
}

std::vector<std::string> splitVectorMain(const std::string& str, const std::string& delim)
{
	std::vector < std::string > tokens;
	size_t prev = 0, pos = 0;
	do
	{
		pos = str.find(delim, prev);
		if (pos == std::string::npos) pos = str.length();
		std::string token = str.substr(prev, pos - prev);
		if (!token.empty()) tokens.push_back(token);
		prev = pos + delim.length();
	} while (pos < str.length() && prev < str.length());
	return tokens;
}

void loadConfigFile() {
	ifstream configFile(gStore->getProperty("dllDirectory")+"mag787.config");

	string line;

	while (getline(configFile, line)) {
		vector<string> configVar = splitVectorMain(line, " ");
		if (configVar.at(0) == "debugMode") {
			gStore->addProperty("debugMode", configVar.at(1));
		}
		else if (configVar.at(0) == "version") {
			gStore->addProperty("version",configVar.at(1));
		}
		else if (configVar.at(0) == "projectJarFile") {
			gStore->addProperty("projectJarFile", configVar.at(1));
		}
	}
}

int  findFiles(XPLMCommandRef       inCommand, XPLMCommandPhase     inPhase, void *               inRefcon) {

	if (inPhase == 0) {
		if (!fileScanning) {
			fileScanning = true;
			thread t5(readDirectory);
			t5.detach();
		}
	}
	return 0;

}


XPLM_API XPLMCommandRef getFiles;

shared_ptr<PluginRunner> getCurrentPlugin() {
	return pluginRunner;
}


PLUGIN_API int XPluginStart(char * outName, char * outSignature, char * outDescription) {

	strcpy(outName, "MAG 787");
	strcpy(outSignature, "mag787.udpexport");
	strcpy(outDescription, "EXPORTS 787 DATA OVER UDP TO JVM");

	dllDirectory = getModuleDirectory();

	gStore->addProperty("dllDirectory", dllDirectory);

	loadConfigFile();
	

	//pdfCommand = XPLMCreateCommand("mag787/efb/docs/pdfConvertor", "CONVERTS PDF FILE TO PNG FOR LOADING INTO SIM");
	//test = XPLMCreateCommand("mag787/efb/docs/addDir", "ADD A DIRECTORY");
	getFiles = XPLMCreateCommand("mag787/efb/docs/searchDir", "searchFiles For Directory");
	
	

	XPLMRegisterCommandHandler(getFiles, findFiles, 0, NULL);
	
	
	gStore->addProperty("Exiting", "false");

	return 1;
}

PLUGIN_API void XPluginStop(void) {
	gStore->setProperty("Exiting", "true");
	if (pluginRunner) {
		pluginRunner->cleanup();
	}
	pluginRunner = nullptr;
	IMG_Quit();
	SDL_Quit();
	XPLMDebugString("[MAG 787] Plugin successfully exited\n");
}

PLUGIN_API int XPluginEnable(void) {

	pluginRunner = make_shared<PluginRunner>();
	pluginRunner->setCurrentPlugin(pluginRunner);
	pluginRunner->initializePlugin();

	gStore->setProperty("Exiting", "false");
	//logger.setLogLevel(Logger::log_error);
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		XPLMDebugString("[JAVA2XP] SDL FAILED TO INTITIALIZE\n");
	}

	if (IMG_Init(IMG_INIT_PNG) < 0) {
		XPLMDebugString("[JAVA2XP] SDL_IMG FAILED TO INTITIALIZE\n");
	}

	if (glewInit() != GLEW_OK) {

		XPLMDebugString("[JAVA2XP] GLEW FAILED TO INTITIALIZE\n");
	}

	
	XPLMDebugString("GRPAHICS LIBRARIES SUCCESFULLY INITED\n");
	return 1;
}

PLUGIN_API void XPluginDisable(void) {
	gStore->setProperty("Exiting", "true");
	pluginRunner->cleanup();
	pluginRunner = nullptr;
	
}

PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFrom, int inMessage, void * inParam) {


}









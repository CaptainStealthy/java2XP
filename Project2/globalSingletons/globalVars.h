#pragma once
#include<string>
namespace GlobalVars {


	extern std::string workingDirectory;

	extern void setWorkingDirectory(std::string dir) {
		workingDirectory = dir;
	}

	extern std::string getWorkingDirectory() {
		return workingDirectory;
	}
}
#pragma once
#include <string>
class Module
{
public:
	Module(std::string moduleName,int moduleID);
	~Module();


private:
	std::string moduleName;
	int moduleID;

};


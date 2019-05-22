#pragma once
#include <string>
#include "utility/Logger.h"
using namespace std;
class startupChecker {

public:
	startupChecker();
	~startupChecker();
	
	bool checkJavaVersion();
	string retrieveJavaPath();
private:
	unique_ptr<Logger> logger;
};

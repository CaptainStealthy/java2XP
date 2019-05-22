#pragma once
#include <string>
#include <XPLMUtilities.h>
#include <ctime>
#include <mutex>
#include <globalSingletons/drefStore.h>

class DrefStore;

using namespace std;
class Logger
{
public:


	static enum log_level{log_debug,log_message,log_error};
	Logger(string className);

	string className;

	void logIntMessage(int integer, string additionalMessage);
	void setLogLevel(log_level level);
	void logString(string message);
	void logDouble(double decimalNumber, string additionalMessage);
	void logDataRef(string dataRefName,string additionalMessage);
	void logMessages();
	
	~Logger();
	
private:
	
	int updateID = 0;

	shared_ptr<DrefStore> store;
	
	bool internalCall = false;
	std::vector<std::string> listOfMessages;
	void insertMessage(std::string message);
	string generateInialHeader();
	log_level current_level;
	
};


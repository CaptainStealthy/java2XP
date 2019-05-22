#include "Logger.h"
#include<XPLMProcessing.h>
#include <vector>
#include <CallbackInterface.h>


Logger::Logger(string className)
{
	this->className = className;
	store = DrefStore::getInstance();
	updateID = CallbackInterface::getInstance()->registerCB(std::bind(&Logger::logMessages, this));
	
}

void Logger::insertMessage(std::string message) {
	
	listOfMessages.push_back(message);
}

void Logger::logMessages() {


	for (std::string message : listOfMessages) {
		XPLMDebugString(message.c_str());
	}
	listOfMessages.clear();
	
}



void Logger::logIntMessage(int integer, string additionalMessage)
{
	internalCall = true;
	string initMessage = generateInialHeader();
	//STRING MESSAGE ALWAYS GOES FIRST BEFORE INTEGER
	initMessage += additionalMessage + to_string(integer);
	initMessage += "\n";
	logString(initMessage);
}

void Logger::setLogLevel(log_level level)
{
	current_level = level;
}

void Logger::logString(string message)
{
	
	if (!internalCall) {
		message = generateInialHeader()+ message+"\n";
	}
	char logMessage[1024];
	
	strcpy(logMessage, message.c_str());
	logMessage[message.length() ] = '\0';

	insertMessage(std::string(logMessage));
	
	internalCall = false;
}

void Logger::logDouble(double decimalNumber, string additionalMessage)
{
	internalCall = true;
	string initMessage = generateInialHeader();
	//STRING MESSAGE ALWAYS GOES FIRST BEFORE INTEGER
	initMessage += additionalMessage + to_string(decimalNumber);
	initMessage += "\n";
	logString(initMessage);
}

void Logger::logDataRef(string dataRefName, string additionalMessage)
{
	internalCall = true;
	string drefValue = "Dref ";
	string initMessage = generateInialHeader();
	initMessage += additionalMessage + drefValue;
	logString(initMessage);
}

string Logger::generateInialHeader() {
	time_t timeNow = time(0);
	tm* now = localtime(&timeNow);
	string timeString = to_string(now->tm_hour) + ":" + to_string(now->tm_min) + ":" + to_string(now->tm_sec);

	string level;
	switch (current_level) {
	case log_debug:
		level = "[DEBUG]";
		break;
	case log_error:
		level = "[ERROR]";
		break;
	case log_message:
		level = "[MESSAGE]";
		break;
	default:
		level = "[UNKWON]";
		break;
	}

	string initialMessage = level + "[Mag 787 " + timeString + "] " + className + ": ";

	return initialMessage;
}


Logger::~Logger()
{
	if (CallbackInterface::getInstance()) {
		CallbackInterface::getInstance()->removeCallback(updateID);
	}
	
}

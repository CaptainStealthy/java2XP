#pragma once
#include <string>
#include <utility/Logger.h>

#include <XPLMDataAccess.h>
class dref
{
public:
	Logger* logger = new Logger("Dataref Handler");
	dref();
	dref(int type,std::string drefName);

	std::string identifier;

	XPLMDataRef drefAccess;
	int drefType;

	virtual std::string getAsString() {
		logInvalidError();
		return "Invalid";
	};
	virtual int getAsInt() {
		logInvalidError();
		return 1;
	};
	virtual double getAsDouble() {
		logInvalidError();
		return -1.0;
	};

	virtual double* getAsDoubleArray() {
		logInvalidError();
		return nullptr;
	};

	virtual void setDoubleArray(double values[], int startOffset) {
		logInvalidError();
	}

	virtual void setDoubleArrayIndex(double value, int index) {
		logInvalidError();
	}

	virtual double* getDoubleArraySpecificIndex(int index1, int index2) {
		logInvalidError();
		return nullptr;
	}

	virtual double getDoubleArrayIndex(int index) {
		logInvalidError();
		return 0.0;
	}

	virtual int* getAsIntArray() {
		logInvalidError();
		return nullptr;
	}

	virtual void setIntArray(int values[], int startOffset) {
		logInvalidError();
	}

	virtual void setIntArraySpecificIndex(int value, int index) {
		logInvalidError();
	}

	virtual int* getIntArraySpecificIndex(int index1, int index2) {
		logInvalidError();
		return nullptr;
	}

	virtual int getIntArrayIndex(int index) {
		logInvalidError();
		return 0;
	}

	virtual void setInt(int value) {
		logInvalidError();
		
	};

	virtual int getArraySize() {
		logInvalidError();
		return 0;
	}

	virtual void setDouble(double value) {
		logInvalidError();
	};

	virtual void setString(std::string value) {
		logInvalidError();
	};

	virtual void update() {};

	void logInvalidError() {
		logger->setLogLevel(Logger::log_error);
		logger->logString(ERROR_MESSAGE_INVALID_TYPE);
	}


	virtual ~dref();

private:
	const std::string ERROR_MESSAGE_INVALID_TYPE = identifier+" :Invalid Operation on dataref type";
};


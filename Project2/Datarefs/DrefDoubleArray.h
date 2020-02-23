
#include "Dref.h"
#include "XPLMDataAccess.h"
#include <string>

class DrefDoubleArray :
	public Dref
{
public:
	DrefDoubleArray(std::string name) :Dref(DrefStore::decimal_array, name) {

	};


	~DrefDoubleArray();

	int getArraySize() {
		return this->sizeOfArray;
	}

	double* getAsDoubleArray() {

		if (!initialized) {
			initialize();
		}

		double* returnArray = new double[sizeOfArray];

		for (int i = 0; i < sizeOfArray; i++) {
			returnArray[i] = arrayPtr[i];
		}

		return returnArray;
	}

	double* getDoubleArraySpecificIndex(int index1, int index2) {

		if (!initialized) {
			initialize();
		}

		int sizeOfArray = index2 - index1;


		if (this->sizeOfArray < index1 + sizeOfArray) {
			logger->setLogLevel(logger->log_error);
			logger->logString("Invalid array paramters. Please provide right size");
			return nullptr;
		}


		double* returnArray = new double[sizeOfArray];


		for (int i = 0; i < sizeOfArray; i++) {
			returnArray[i] = arrayPtr[i];
		}

		return returnArray;

	}

	double getDoubleArrayIndex(int index) {
		
		if (!initialized) {
			initialize();
		}

		if (index > sizeOfArray - 1) {
			logger->setLogLevel(logger->log_error);
			logger->logString("Invalid array paramters. Please provide right size");
			return 0.0;
		}

		return arrayPtr[index];
	}

	void setDoubleArray(double values[], int startOffset) {

		if (!initialized) {
			initialize();
		}

		int lengthOfArray = sizeof(values) / sizeof(*values);

		if (startOffset + lengthOfArray > sizeOfArray) {
			logger->setLogLevel(logger->log_error);
			logger->logString("Invalid array paramters. Please provide right size");
			return;
		}

		int arrayCounter = 0;

		for (int i = startOffset; i < startOffset + lengthOfArray; i++) {
			arrayPtr[i] = values[arrayCounter];
			arrayCounter++;
		}

		writeNextLoopArray = true;
		
	}


	void setDoubleArrayIndex(double value, int index) {

		if (!initialized) {
			initialize();
		}

		if (index > sizeOfArray - 1) {
			logger->setLogLevel(logger->log_error);
			logger->logString("Invalid array paramters. Please provide right size");
			return;
		}

		arrayPtr[index] = value;

		valueToSet = value;
		indexToSet = index;

		writeNextLoopValue = true;
	}

	void update() {
		if (!initialized) {
			initialize();
		}

		if (!writeNextLoopValue && !writeNextLoopArray) {
			XPLMGetDatavf(drefAccess, arrayPtr, 0, sizeOfArray);
		}
		else if (writeNextLoopValue) {

			float * value = new float[1];
			value[0] = valueToSet;
			XPLMSetDatavf(drefAccess, value, indexToSet, 1);

			delete[] value;
			writeNextLoopValue = false;
		}
		else if (writeNextLoopArray) {
			XPLMSetDatavf(drefAccess, arrayPtr, 0, sizeOfArray);
			writeNextLoopArray = false;
		}

	}


private:

	void initialize() {
		sizeOfArray = XPLMGetDatavf(drefAccess, NULL, 0, 0);
		arrayPtr = new float[sizeOfArray];
		initialized = true;
	}
	
	float* arrayPtr;

	int sizeOfArray = 0;

	float valueToSet = 0.0;
	int indexToSet = 0;


	boolean initialized = false;
	boolean writeNextLoopValue = false;
	boolean writeNextLoopArray = false;

};

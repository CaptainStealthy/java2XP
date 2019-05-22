#include "DrefIntArray.h"






DrefIntArray::~DrefIntArray()
{
}

int * DrefIntArray::getAsIntArray()
{
	if (!initialized) {
		initialize();
	}

	int* requestedArray = new int[sizeOfArray];

	for (int i = 0; i < sizeOfArray; i++) {
		requestedArray[i] = arrayPtr[i];
	}

	return requestedArray;
}

void DrefIntArray::setIntArray(int values[], int startOffset)
{

	if (!initialized) {
		initialize();
	}

	int lengthOfValues = sizeof(values) / sizeof(*values);

	int maxValueIndex = startOffset + lengthOfValues;

	if (maxValueIndex > sizeOfArray) {
		logger->setLogLevel(logger->log_error);
		logger->logString("Invalid array paramters. Please provide right size");
		return;
	}

	for (int i = startOffset; i < maxValueIndex; i++) {
		arrayPtr[i] = values[i];
	}

	writeNextLoopArray = true;

}

void DrefIntArray::setIntArraySpecificIndex(int value, int index)
{

	if (!initialized) {
		initialize();
	}

	if (index > sizeOfArray - 1) {
		logger->setLogLevel(logger->log_error);
		logger->logString("Invalid array paramters. Please provide right size");
		return;
	}

	valueToWrite = value;
	indexToSet = index;
	arrayPtr[index] = value;

	writeNextLoop = true;

}

int * DrefIntArray::getIntArraySpecificIndex(int index1, int index2)
{

	if (!initialized) {
		initialize();
	}

	int size = index2 - index1;

	int maxValue = index1 + size;

	if (maxValue > sizeOfArray - 1) {
		logger->setLogLevel(logger->log_error);
		logger->logString("Invalid array paramters. Please provide right size");
		return nullptr;
	}

	int * requestedValues = new int[size];

	for (int i = index1; i < maxValue; i++) {
		requestedValues[i] = arrayPtr[i];
	}

	return requestedValues;
}

int DrefIntArray::getIntArrayIndex(int index)
{

	if (!initialized) {
		initialize();
	}

	if (index > sizeOfArray - 1) {
		logger->setLogLevel(logger->log_error);
		logger->logString("Invalid array paramters. Please provide right size");
	}

	return arrayPtr[index];
}

int DrefIntArray::getArraySize()
{
	return this->sizeOfArray;
}

void DrefIntArray::update()
{

	if (!initialized) {
		initialize();
	}
	
}


void DrefIntArray::initialize()
{
	sizeOfArray = XPLMGetDatavi(drefAccess, NULL, 0, 0);
	arrayPtr = new int[sizeOfArray];
	initialized = true;
}

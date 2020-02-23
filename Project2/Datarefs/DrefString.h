#include "Dref.h"
#include<Datarefs/drefStore.h>
#include<XPLMDataAccess.h>
#include<string>
#include <XPLMProcessing.h>
#include <atomic>

class DrefString :public Dref {

public:
	DrefString(std::string name) :Dref(DrefStore::string_dref, name) {


	};

	std::string getAsString() {

		mtxDref.lock();

		//read and create copy of dataref before unlocking 
		std::string value = std::string(currentValue);

		mtxDref.unlock();

		return value;
	}

	void setString(std::string value) {
		
		mtxDref.lock();
		valueToSetNextLoop = value;
		valueChanged = true;
		mtxDref.unlock();
	}

	void update() {

		mtxDref.lock();

		if (valueChanged) {
			
			const char* valueString = valueToSetNextLoop.c_str();

			XPLMSetDatab(drefAccess, (void *)valueString, 0, strlen(valueString));
			valueChanged = false;
		}

		char valueRetrieved[1024];
		int size = XPLMGetDatab(drefAccess, NULL, 0, NULL);

		XPLMGetDatab(drefAccess, &valueRetrieved, 0, size);
		valueRetrieved[size] = 0;

		currentValue = valueRetrieved;

		mtxDref.unlock();
	}


private:
	std::string currentValue;
	std::string valueToSetNextLoop;
	std::atomic_bool valueChanged = false;
	std::mutex mtxDref;
};



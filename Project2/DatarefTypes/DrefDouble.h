#include<globalSingletons/dref.h>
#include<globalSingletons/drefStore.h>
#include<XPLMDataAccess.h>
#include<string>
#include <XPLMProcessing.h>

class DrefDouble :public dref {


public:
	DrefDouble(std::string name) :dref(DrefStore::decimal, name) {


	};

	double getAsDouble() {
		return currentValue;
	}

	void setDouble(double value) {
		valueToWrite = value;
		writeNextLoop = true;
		currentValue = valueToWrite;
	}

	void update() {
		if (writeNextLoop) {
			XPLMSetDatad(drefAccess, valueToWrite);
			writeNextLoop = false;
		}
		else {
			currentValue = XPLMGetDatad(drefAccess);
		}
	}


private:
	double currentValue = 0;
	double valueToWrite = 0;
	bool writeNextLoop = false;
};



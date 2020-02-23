#include "Dref.h"
#include<Datarefs/drefStore.h>
#include<XPLMDataAccess.h>
#include<string>
#include <XPLMProcessing.h>
#include <atomic>

class DrefDouble :public Dref {


public:
	DrefDouble(std::string name) :Dref(DrefStore::decimal, name) {


	};

	double getAsDouble() {
		return cachedValue;
	}

	void setDouble(double value) {
		valueToSetnextLoop = value;
		valueChanged = true;
	}

	void update() {
		if (valueChanged) {
			XPLMSetDatad(this->drefAccess, valueToSetnextLoop);
			valueChanged = false;
		}
		cachedValue = XPLMGetDatad(this->drefAccess);
	}


private:
	std::atomic<double> valueToSetnextLoop;
	std::atomic<double> cachedValue;
	std::atomic_bool valueChanged = false;
};



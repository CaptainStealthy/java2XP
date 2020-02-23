#include "Dref.h"
#include<Datarefs/drefStore.h>
#include<XPLMDataAccess.h>
#include<string>
#include <XPLMProcessing.h>
#include <atomic>

class DrefInt :public Dref {
	
public:

	DrefInt(std::string name) :Dref(DrefStore::integer, name) {
		

	};

	int getAsInt() {
		return cachedValue;
	}

	void setInt(int value) {
		valueToSetNextLoop = value;
		valueChanged = true;
	}

	void update() {
		if (valueChanged) {
			XPLMSetDatai(this->drefAccess, valueToSetNextLoop);
			valueChanged = false;
		}
		cachedValue = XPLMGetDatai(this->drefAccess);
	}

	
private:
	std::atomic_int valueToSetNextLoop;
	std::atomic_int cachedValue;
	std::atomic_bool valueChanged = false;

	
}; 



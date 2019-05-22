#include<globalSingletons/dref.h>
#include<globalSingletons/drefStore.h>
#include<XPLMDataAccess.h>
#include<string>
#include <XPLMProcessing.h>

class DrefInt :public dref {
	
public:

	DrefInt(std::string name) :dref(DrefStore::integer, name) {
		

	};

	int getAsInt() {
		return currentValue;
	}

	void setInt(int value) {
		valueToWrite = value;
		currentValue = valueToWrite;
		writeNextLoop = true;
	}

	void update() {
		if (writeNextLoop) {
			XPLMSetDatai(drefAccess, valueToWrite);
			
			writeNextLoop = false;
		}
		else {
			currentValue = XPLMGetDatai(drefAccess);
		}
	}

	
private:
	int currentValue = 0;
	int valueToWrite = 0;
	bool writeNextLoop = false;
}; 



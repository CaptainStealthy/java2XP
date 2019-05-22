#include<globalSingletons/dref.h>
#include<globalSingletons/drefStore.h>
#include<XPLMDataAccess.h>
#include<string>
#include <XPLMProcessing.h>

class DrefString :public dref {

public:
	DrefString(std::string name) :dref(DrefStore::string_dref, name) {


	};

	std::string getAsString() {
		return currentValue;
	}

	void setString(std::string value) {
		valueToWrite = value;
		writeNextLoop = true;
		currentValue = valueToWrite;
	}

	void update() {
		if (writeNextLoop) {
			XPLMSetDatab(drefAccess,(void *)valueToWrite.c_str(),0,strlen(valueToWrite.c_str()));
			
			writeNextLoop = false;
		}
		else {
			char valueRetrieved[1024];
			int size = XPLMGetDatab(drefAccess,NULL, 0, NULL);

			XPLMGetDatab(drefAccess, &valueRetrieved, 0, size);
			valueRetrieved[size] = 0;

			currentValue = valueRetrieved;
		}
	}


private:
	std::string currentValue ="";
	std::string valueToWrite = "";
	bool writeNextLoop = false;
};



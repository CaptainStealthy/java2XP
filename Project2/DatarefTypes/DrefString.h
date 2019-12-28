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
		char valueRetrieved[1024];
		int size = XPLMGetDatab(drefAccess, NULL, 0, NULL);

		XPLMGetDatab(drefAccess, &valueRetrieved, 0, size);
		valueRetrieved[size] = 0;

		currentValue = valueRetrieved;
		return currentValue;
	}

	void setString(std::string value) {

		const char* valueC = value.c_str();

		XPLMSetDatab(drefAccess, (void *)valueC, 0, strlen(valueC));
	}

	void update() {
	
	}


private:
	std::string currentValue ="";
	
};



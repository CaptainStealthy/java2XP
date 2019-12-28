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
		return XPLMGetDataf(this->drefAccess);
	}

	void setDouble(double value) {
		XPLMSetDataf(this->drefAccess, value);
	}

	void update() {
		
	}


private:

};



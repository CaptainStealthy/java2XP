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
		return XPLMGetDatai(drefAccess);
	}

	void setInt(int value) {
		XPLMSetDatai(this->drefAccess, value);
	}

	void update() {
		
	}

	
private:
	
}; 



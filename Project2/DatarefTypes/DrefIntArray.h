
#include "globalSingletons/dref.h"
#include "XPLMDataAccess.h"
#include <string>

class DrefIntArray :
	public dref
{
public:
	
	DrefIntArray(std::string name) :dref(DrefStore::int_array, name) {};
	~DrefIntArray();

	int* getAsIntArray();

	void setIntArray(int values[], int startOffset);

	void setIntArraySpecificIndex(int value, int index);

	int* getIntArraySpecificIndex(int index1, int index2);

	int getIntArrayIndex(int index);

	int getArraySize();

	void update();


private:
	DrefIntArray() {};

	void initialize();

	int* arrayPtr;

	int sizeOfArray = 0;

	int valueToWrite = 0;
	int indexToSet = 0;

	boolean initialized = false;
	boolean writeNextLoop = false;
	boolean writeNextLoopArray = false;



};

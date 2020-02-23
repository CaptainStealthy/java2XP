#include "Dref.h"


#include <string>



Dref::Dref() {

}

Dref::Dref(int type,std::string drefName)
{
	drefAccess = XPLMFindDataRef(drefName.c_str());
	drefType=type;
	identifier = drefName;
}


Dref::~Dref()
{
}

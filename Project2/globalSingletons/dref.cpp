#include "dref.h"


#include <string>



dref::dref() {

}

dref::dref(int type,std::string drefName)
{
	drefAccess = XPLMFindDataRef(drefName.c_str());
	drefType=type;
	identifier = drefName;
}


dref::~dref()
{
}

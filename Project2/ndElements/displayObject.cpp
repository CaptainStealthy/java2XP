#include "displayObject.h"

#define APT 1
#define WPT 2
#define NAV 3
#define LEG 4

using namespace std;

double bfl;
double dfl;

displayObject::displayObject() {

}

displayObject::displayObject(double lon, double lat, string ident, int objectType)
{
	longitude = lon;
	latitude = lat;
	name = ident;
	type = objectType;
}

displayObject::displayObject(double lon, double lat, string legName, double dfl1, double bfl1) {
	//THIS IS AN OVERLOADED CONSTRUCTOR FOR A LEG OBJECT
	longitude = lon;
	latitude = lat;
	name = legName;
	bfl = bfl1;
	dfl = dfl1;
}


displayObject::~displayObject()
{
	free(this);
}

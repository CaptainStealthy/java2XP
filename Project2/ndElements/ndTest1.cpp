#include "ndTest1.h"
#include "displayObject.h"
#include <map>
//#include "acfutils\geom.h"
#include <cmath>
#include <XPLMDataAccess.h>
#include <vector>
using namespace std;

#define LEFT_SIDE 1
#define RIGHT_SIDE 2

GLuint textureID;

typedef XPLMDataRef dref;

geo_pos2_t cacheLocation;

//DATAREFS FOR NAVIGATION INFORMATION

XPLM_API dref currentHdg;
XPLM_API dref MagneticDeviation;
XPLM_API dref planeLat;
XPLM_API dref planeLon;
XPLM_API dref ndRange;

boolean airportsStored = false;

double rotation;

mt_cairo_render_t *ndRender;

map<string, displayObject> apts;

vector<displayObject> aptsInRange;

fpp_t projectionFormula;




ndTest1::ndTest1(int length, int height,int type,map<string,displayObject> aptMap,int side)
{	
	if (side == LEFT_SIDE) {
		ndRange = XPLMFindDataRef("aero787/cockpit/nd/rangeL");
	}
	else if (side == RIGHT_SIDE) {
		ndRange = XPLMFindDataRef("aero787/cockpit/nd/rangeR");
	}
	apts = aptMap;
	this->ND_TYPE = type;
	canvasLength = length;
	canvasHeight = height;

	//ndRender = mt_cairo_render_init(canvasLength, canvasHeight, 3.0, NULL, NULL, NULL, NULL);
}

//mt_cairo_render_cb_t renderSmall() {
//
//}

//WE NEED A FUNCTION TO DRAW ALL THE AIRPORTS IN RANGE

void ndTest1::findAptsInRange() {
	//WE NEED TO DO THIS ONCE EVERY 640 NM
	//EVERY 320 NM WE FIND AIRPOTS WIHTIN 1600 NM OF THE AIRPLANE TO CACHE AND DRAW
	for (auto &airport : apts) {
	//	geo_pos2_t planeLocation{ XPLMGetDatad(planeLat),XPLMGetDatad(planeLon) };
		
		//geo_pos2_t artifactLocation{ airport.second.latitude,airport.second.longitude };

		double distance = 0;

		if (distance < 1600.0) {
			aptsInRange.push_back(airport.second);
		}

	}
}
void ndTest1::update() {
	//CHECK IF AIRPORTS HAVE BEEN STORED OR NOT
	//geo_pos2_t planeLocation{ XPLMGetDatad(planeLat),XPLMGetDatad(planeLon) };
	if (!airportsStored) {
		findAptsInRange();
		cacheLocation={ XPLMGetDatad(planeLat),XPLMGetDatad(planeLon) };

		airportsStored = true;
	}
	double distanceSinceLastCache = 0;//(gc_distance(planeLocation, cacheLocation));
	if (distanceSinceLastCache > 640.0) {
		airportsStored = false;
	}
	
	rotation = deg2Rad((XPLMGetDatad(currentHdg) - XPLMGetDatad(MagneticDeviation)));
	//projectionFormula = gnomo_fpp_init(planeLocation, rotation, &wgs84, false);
	

}



double ndTest1::deg2Rad(double degrees) {
	return (degrees* 0.0174533);
}



ndTest1::~ndTest1()
{
}



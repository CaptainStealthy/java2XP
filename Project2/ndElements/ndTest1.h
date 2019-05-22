#pragma once
typedef int length;

#define NDSMALL 1
#define NDWIDE 2
#include "displayObject.h"
#include<map>
//#include <acfutils\mt_cairo_render.h>

using namespace std;

class ndTest1
{
public:
	 int ND_TYPE;
	length canvasLength;
	int canvasHeight;
	ndTest1(int length, int height,int type, map<string,displayObject> aptMap,int side);
	void update();
	~ndTest1();
private:
	double deg2Rad(double degrees);
	void findAptsInRange();
};


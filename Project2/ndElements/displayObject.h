#pragma once
#include<string>
using namespace std;
class displayObject
{
public:
	double longitude;
	double latitude;
	string name;
	int type;
	displayObject();
	displayObject(double lon,double lat,string ident,int objectType);
	displayObject(double lon, double lat, string legName, double dfl1, double bfl1);
	~displayObject();
};


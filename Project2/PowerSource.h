#pragma once

#include <string>

using namespace std;

class PowerSource
{
public:
	PowerSource(string ID,double maxPowerWatts);

	double powerGenerating();

	void update();

	~PowerSource();


private:
	double powerGenerated = 0.0;

	bool isProvidingPower = false;


};


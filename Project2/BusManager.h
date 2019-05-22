#pragma once

#include <Bus.h>
#include <vector>
#include <PowerSource.h>

using namespace std;

class BusManager
{
public:

	static BusManager& getInstance() {
		static BusManager bm;
		return bm;
	}

	void addBus(Bus bus) {
		allBusses.push_back(bus);
	}

	void addPowerSource(PowerSource source) {
		allPowerSources.push_back(source);
	}

	void computeLoad();

	void computeAvailPower();

	void performLoadShed();

	void update();


	~BusManager();

private:
	vector<Bus> allBusses;
	vector<PowerSource> allPowerSources;

	double totalPowerAvailWatts;
	double totalConsumptionWatts;

	BusManager();

};


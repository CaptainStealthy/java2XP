#pragma once

#include <vector>
#include <Relay.h>
#include <electrical/LoadType.h>
#include <string>



class Bus
{
public:
	Bus(busType type, std::string busID) {
		this->type = type;
		this->busID = busID;
	};

	~Bus();

	virtual void addRelay(Relay& relay) {
		allRelays.push_back(relay);
	}

	double calculatePriorityFactor();

	void calculateLoadShed();

	double calculatePowerDraw();

private:

	std::string busID;

	double priorityFactor;

	std::vector<Relay> allRelays;

	double powerDrawWatts;

	busType type;



};


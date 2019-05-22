#include "Bus.h"




Bus::~Bus()
{
}

double Bus::calculatePriorityFactor()
{
	priorityFactor = 0.0;

	for (Relay relay : allRelays) {
		relay.recalculatePriority();
		priorityFactor+=relay.getPriorityFactor();
	}

	return this->priorityFactor;
}


void Bus::calculateLoadShed()
{
	double lowestPriorityFactor = 1000.0;
	Relay* lowestPriorityRelay;

	for (Relay relay : allRelays) {
		relay.recalculatePriority();
		if (relay.getPriorityFactor() < lowestPriorityFactor) {
			lowestPriorityFactor = relay.getPriorityFactor();
			lowestPriorityRelay = &relay;
		}
	}

	lowestPriorityRelay->shedLoad();
}

double Bus::calculatePowerDraw()
{
	powerDrawWatts = 0.0;
	for (Relay relay : allRelays) {
		powerDrawWatts+=relay.getWatts();
	}

	return this->powerDrawWatts;
}

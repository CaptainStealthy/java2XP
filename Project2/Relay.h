#pragma once
#include <electrical/LoadType.h>
#include <string>
#include <vector>
#include <Load.h>

class Relay
{
public:
	Relay();
	~Relay();

	double getWatts();
	double getPriorityFactor();
	int getNumberOfConnections();

	void shedLoad();

	relayType getRelayType();
	std::string getRelayID();

	void recalculatePriority() {
		int calculatedPriority = 0;
		for (Load* load : loads) {
			calculatedPriority += load->getPriorityFactor();
		}
		this->priorityFactor =(double) calculatedPriority;
	}

	void addLoad(Load* load) {

		//SWITCH TO CHECK IF THE LOAD PROVIDED ACTUALLY MATCHES RELAY TYPE.
		switch (load->getLoadType())
		{
		case LOAD_AC:
			if (this->type != RELAY_AC) {
				//TODO: Error message here
				return;
			}
		case LOAD_DC:
			if (this->type != RELAY_DC) {
				//TODO: Error message here
				return;
			}
		default:
			break;
		}
		connections++;
		loads.push_back(load);
	}

private:

	std::string relayID;
	int connections;
	relayType type;
	double powerUsedWatts;
	double priorityFactor;
	std::vector<Load*> loads;


};


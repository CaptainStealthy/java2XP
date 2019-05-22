#include "Relay.h"



Relay::Relay()
{
}


Relay::~Relay()
{
}

double Relay::getWatts()
{
	powerUsedWatts = 0.0;
	for (Load* load : loads) {
		powerUsedWatts = load->getPowerDraw();
	}
}

double Relay::getPriorityFactor()
{
	return 0.0;
}

int Relay::getNumberOfConnections()
{
	return 0;
}

void Relay::shedLoad()
{
}

relayType Relay::getRelayType()
{
	return relayType();
}

std::string Relay::getRelayID()
{
	return std::string();
}

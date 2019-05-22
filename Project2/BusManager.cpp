#include "BusManager.h"



BusManager::BusManager()
{
}


void BusManager::computeLoad()
{

	totalConsumptionWatts = 0.0;

	for (Bus bus : allBusses) {
		totalConsumptionWatts+=bus.calculatePowerDraw();
	}


}

void BusManager::computeAvailPower()
{
	totalPowerAvailWatts = 1000.0;

	for (PowerSource source : allPowerSources) {
		totalPowerAvailWatts += source.powerGenerating();
	}
}


/*
This method is the core of the load shedding system. It is supposed to 
calculate which Bus has the lowest priority factor and the highest power usage. 
**/

void BusManager::performLoadShed()
{
	//THIS IS THE HIGHEST PRIORITY FACTOR POSSIBLE
	double lowestPriorityFactor = 1000.0;
	Bus* busWithLowestPriority;

	//WE CAN ONLY CONSUME WHAT WE HAVE 
	while (totalConsumptionWatts > totalPowerAvailWatts) {
		for (Bus bus : allBusses) {

			double priorityFactor;
			if ( priorityFactor = bus.calculatePriorityFactor() < lowestPriorityFactor) {
				lowestPriorityFactor = priorityFactor;
				busWithLowestPriority = &bus;
			}
		}

		busWithLowestPriority->calculateLoadShed();
		computeLoad();
		computeAvailPower();
	}

}

void BusManager::update()
{
	computeLoad();
	computeAvailPower();
	performLoadShed();
}

BusManager::~BusManager()
{
}

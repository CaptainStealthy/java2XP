#pragma once
#include <string>
#include <electrical/LoadType.h>

class Load
{
public:
	Load(std::string loadID, int priorityFactor,double powerDrawWatts) {
		this->priorityFactor = priorityFactor;
		this->loadID = loadID;
		this->loadShed = false;
	};

	~Load();

	int getPriorityFactor() {
		return this->priorityFactor;
	}

	std::string getLoadID() {
		return this->loadID;
	}

	void shedLoad() {
		this->loadShed = true;
	}

	bool isAlreadyShed() {
		return this->loadShed;
	}

	double getPowerDraw() {
		return this->powerDrawWatts;
	}

	virtual loadType getLoadType() = 0;

	void powerUpLoad() {
		this->loadShed = false;
	}

private:
	int priorityFactor;
	std::string loadID;
	bool loadShed;
	double powerDrawWatts;

};


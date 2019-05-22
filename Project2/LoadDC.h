#pragma once
#include "Load.h"
#include "electrical/LoadType.h"
class LoadDC :
	public Load
{
public:

	LoadDC(std::string loadID, int priorityFactor,double powerDrawWatts) :Load(loadID, priorityFactor,powerDrawWatts) {
		this->type = loadType::LOAD_DC;
	}

	loadType getLoadType() {
		return loadType::LOAD_DC;
	}

	~LoadDC();

private:
	loadType type;

};


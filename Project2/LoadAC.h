#pragma once
#include "Load.h"
#include <electrical/LoadType.h>
class LoadAC :
	public Load
{
public:
	LoadAC(std::string loadID,int priorityFactor,double powerDrawWatts) :Load(loadID,priorityFactor,powerDrawWatts) {
		this->type = loadType::LOAD_AC;
		
	};

	loadType getLoadType() {
		return loadType::LOAD_AC;
	}

	~LoadAC();

private:
	loadType type;
	
};


#pragma once
class Module
{
public:
	Module();

	~Module();

	virtual void update() {};

	virtual void draw() {};

	void setModuleID(int ID) {
		this->moduleID = ID;
	}

private:
	unsigned int moduleID;
	
};


#include "DrefCommand.h"
#include<globalSingletons/drefStore.h>
#include <vector>
using namespace std;



drefStore *storeAccess = drefStore::getInstance();

DrefCommand::DrefCommand(std::future<std::string>* &consumerObject, dref dataRef, const command_types commandType) {
	consumerObject = new future<std::string>(promiseToDoWork.get_future());
	this->commandType = commandType;
	this->dataRef = dataRef;
};

DrefCommand::DrefCommand( dref dataRef, std::string value, command_types commandType) {
	
	this->commandType = commandType;
	this->value = value;
	this->dataRef = dataRef;
};

void DrefCommand::executeCommand()
{
	if (commandType==set) {
		switch (dataRef.drefType) {
		case string_dref:
			storeAccess->setString(dataRef.drefAccess, value);
			break;
		case integer:
			storeAccess->setInt(dataRef.drefAccess, stoi(value));
			break;
		case decimal:
			storeAccess->setDecimal(dataRef.drefAccess, stod(value));
			break;
		}
	}
	else if(commandType == get) {
		switch (dataRef.drefType) {
		case string_dref:
			this->promiseToDoWork.set_value(storeAccess->getString(dataRef.drefAccess));
			break;
		case integer:
			this->promiseToDoWork.set_value(to_string(storeAccess->getInt(dataRef.drefAccess)));
			break;
		case decimal:
			this->promiseToDoWork.set_value(to_string(storeAccess->getDecimal(dataRef.drefAccess)));
			break;
		}
	}
}

DrefCommand::~DrefCommand()
{
}

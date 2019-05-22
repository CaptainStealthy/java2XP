#include "drefStore.h"
#include "dref.h"
#include <DatarefTypes/DrefDouble.h>
#include <DatarefTypes/DrefInt.h>
#include <DatarefTypes/DrefIntArray.h>
#include <DatarefTypes/DrefString.h>
#include <DatarefTypes/DrefDoubleArray.h>
#include "DrefCommand.h"
#include <XPLMDataAccess.h>
#include <XPLMProcessing.h>
#include <CallbackInterface.h>


std::vector<std::string> splitVector(const std::string& str, const std::string& delim)
{
	std::vector < std::string > tokens;
	size_t prev = 0, pos = 0;
	do
	{
		pos = str.find(delim, prev);
		if (pos == std::string::npos) pos = str.length();
		std::string token = str.substr(prev, pos - prev);
		if (!token.empty()) tokens.push_back(token);
		prev = pos + delim.length();
	} while (pos < str.length() && prev < str.length());
	return tokens;
}

static enum drefTypes {
	integer,
	decimal,
	string_dref,
	int_array,
	decimal_array
};


std::map<std::string, drefTypes> types = {
	{std::string("integer"), integer},
	{std::string("decimal"), decimal},
	{std::string("string"), string_dref},
	{std::string("int_array"), int_array},
	{std::string("decimal_array") , decimal_array}
};

shared_ptr<DrefStore> DrefStore::storeInstance = nullptr;


shared_ptr<DrefStore> DrefStore::getInstance()
{
	return storeInstance;
}

shared_ptr<DrefStore> DrefStore::createNewStore()
{
	storeInstance = make_shared<DrefStore>();
	return storeInstance;
}

void DrefStore::initialize() {
	//add("decimal_array", "sim/aircraft/parts/acf_gear_leglen", "testRef");
	
}

void DrefStore::stop() {
	storeInstance = nullptr;
}


void  DrefStore::update() {
	std::map<std::string, std::shared_ptr<dref>>::iterator it = drefStore.begin();

	while (it != drefStore.end()) {
		it->second->update();
		it++;
	}

}

DrefStore::DrefStore()
{
	CallbackInterface::getInstance()->registerCB(std::bind(&DrefStore::update, this));
}


void DrefStore::add(std::string type, std::string simLocation, std::string name) {

	switch (types[type]) {

	case integer:
	{
		dref* test = new DrefInt(simLocation);
		drefStore.insert(std::make_pair(name, test));
		break;
	}
	case decimal:
	{
		dref* test = new DrefDouble(simLocation);
		drefStore.insert(std::make_pair(name, test));
		break;
	}
	case string_dref:
	{
		dref* test = new DrefString(simLocation);
		drefStore.insert(std::make_pair(name, test));
		break;
	}
	case int_array:
		/*DrefStore[command.at(3)] = dref(int_array, command.at(2));
		break;*/
		break;
	case decimal_array:
		dref* decimalDref = new DrefDoubleArray(simLocation);
		drefStore.insert(std::make_pair(name, decimalDref));
		break;
	}


}

void DrefStore::setInt(std::string name, int value)
{
	set(name, to_string(value));
}

void DrefStore::setDecimal(std::string name, double value)
{
	set(name, to_string(value));
}

void DrefStore::setString(std::string name, std::string value)
{
	set(name, value);
}

void DrefStore::remove(std::string name)
{
	if (drefStore.count(name) > 0) {
		drefStore.erase(name);
	}
}

int DrefStore::getInt(std::string name) {
	shared_ptr<dref> drefAccess = drefStore[name];
	return drefAccess->getAsInt();
}

double DrefStore::getDecimal(std::string name)
{
	shared_ptr<dref> drefAccess = drefStore[name];
	return drefAccess->getAsDouble();
}

std::string DrefStore::getString(std::string name)
{
	shared_ptr<dref> drefAccess = drefStore[name];
	return drefAccess->getAsString();
}

double * DrefStore::getDecimalArray(std::string name)
{
	shared_ptr<dref> drefAccess = drefStore[name];
	return drefAccess->getAsDoubleArray();
}

double * DrefStore::getDecimalArraySpecificIndices(std::string name, int index1, int index2)
{
	shared_ptr<dref> drefAccess = drefStore[name];
	return drefAccess->getDoubleArraySpecificIndex(index1,index2);
}

double DrefStore::getDeimalArrayIndex(std::string name, int index)
{
	shared_ptr<dref> drefAccess = drefStore[name];
	return drefAccess->getDoubleArrayIndex(index);
}



void DrefStore::setDecimalArray(std::string name,double values[], int offset)
{

	shared_ptr<dref> drefAccess = drefStore[name];

	drefAccess->setDoubleArray(values, offset);

}

void DrefStore::setDecimalArrayIndex(std::string name,double value, int index)
{
	shared_ptr<dref> drefAccess = drefStore[name];
	
	drefAccess->setDoubleArrayIndex(value, index);
}


void DrefStore::logNotFound(string name) {
	logger->setLogLevel(Logger::log_error);
	logger->logString(" Dataref "+name+" Not found / initialized. Please initiaize before performing any operations.");
}

void DrefStore::logIncorrectType() {
	logger->setLogLevel(Logger::log_error);
	logger->logString(" Incorrect operation on wrong dataref type. ");
}

void DrefStore::logUnsupportedMethod() {
	logger->setLogLevel(Logger::log_error);
	logger->logString(" Unsupported method for array type! ");
}


shared_ptr<dref> DrefStore::getDrefReference(std::string name)
{
	return drefStore.at(name);
}

void DrefStore::set(std::string name,std::string value) {
	
	int dref_exists = drefStore.count(name);
	//WE DIDN'T FIND THE DATAREF, DONT SET ANYTHING
	if (dref_exists == 0) {
		return;
	}
	shared_ptr<dref> drefSelected = drefStore[name];

	switch (drefSelected->drefType) {
	case integer:
	{
		drefSelected->setInt(stoi(value));
	}
		break;
	case decimal:
	{
		drefSelected->setDouble(stod(value));
	}
		break;
	case string_dref:
	{
		drefSelected->setString(value);
		break;
	}
	case int_array:
	{
		logUnsupportedMethod();
	}
		break;
	case decimal_array:
	{
		logUnsupportedMethod();
		break;
	}

	}
}

DrefStore::~DrefStore()
{

}

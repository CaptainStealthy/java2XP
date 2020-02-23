#include "drefStore.h"
#include "Dref.h"
#include <Datarefs/DrefDouble.h>
#include <Datarefs/DrefInt.h>
#include <Datarefs/DrefIntArray.h>
#include <Datarefs/DrefString.h>
#include <Datarefs/DrefDoubleArray.h>
#include <XPLMDataAccess.h>
#include <XPLMProcessing.h>
#include <CallbackInterface.h>


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


void DrefStore::initialize() {
	
}


void  DrefStore::update() {
	std::map<std::string, std::shared_ptr<Dref>>::iterator it = drefStore.begin();

	while (it != drefStore.end()) {
		it->second->update();
		it++;
	}

}

DrefStore::DrefStore()
{
	CallbackInterface::getInstance()->registerCB(std::bind(&DrefStore::update, this));
}


void DrefStore::add(const std::string& type, const std::string& simLocation, const std::string& name) {

	switch (types[type]) {

	case integer:
	{
		std::shared_ptr<Dref> drefInt = std::make_shared<DrefInt>(simLocation);
		drefStore.insert(std::make_pair(name, drefInt));
		break;
	}
	case decimal:
	{
		std::shared_ptr<Dref> drefDouble = std::make_shared<DrefDouble>(simLocation);
		drefStore.insert(std::make_pair(name, drefDouble));
		break;
	}
	case string_dref:
	{
		std::shared_ptr<Dref> drefString = std::make_shared<DrefString>(simLocation);
		drefStore.insert(std::make_pair(name, drefString));
		break;
	}
	case int_array:
	{
		std::shared_ptr<Dref> drefIntArray = std::make_shared<DrefIntArray>(simLocation);
		break;
	}
	case decimal_array:
	{
		std::shared_ptr<Dref> drefDoubleArray = std::make_shared<DrefDoubleArray>(simLocation);
		drefStore.insert(std::make_pair(name, drefDoubleArray));
		break;
	}
	}


}

void DrefStore::setInt(const std::string& name, int value)
{
	if (!drefExists(name)) {
		return;
	}

	shared_ptr<Dref> drefAccess = drefStore[name];
	drefAccess->setInt(value);

}

void DrefStore::setDecimal(const std::string& name, double value)
{
	if (!drefExists(name)) {
		return;
	}

	shared_ptr<Dref> drefAccess = drefStore[name];
	drefAccess->setDouble(value);
}

void DrefStore::setString(const std::string& name, const std::string& value)
{
	if (!drefExists(name)) {
		return;
	}

	shared_ptr<Dref> drefAccess = drefStore[name];
	drefAccess->setString(value);
}

void DrefStore::remove(const std::string& name)
{

	if (!drefExists(name)) {
		return;
	}

	drefStore.erase(name);
	
}

int DrefStore::getInt(const std::string& name) {

	if (!drefExists(name)) {
		return 0;
	}

	shared_ptr<Dref> drefAccess = drefStore[name];
	return drefAccess->getAsInt();
}

double DrefStore::getDecimal(const std::string& name)
{

	if (!drefExists(name)) {
		return 0.0;
	}

	shared_ptr<Dref> drefAccess = drefStore[name];
	return drefAccess->getAsDouble();
}

std::string DrefStore::getString(const std::string& name)
{

	if (!drefExists(name)) {
		return "";
	}

	shared_ptr<Dref> drefAccess = drefStore[name];
	return drefAccess->getAsString();
}

double * DrefStore::getDecimalArray(const std::string& name)
{

	if (!drefExists(name)) {
		return nullptr;
	}

	shared_ptr<Dref> drefAccess = drefStore[name];
	return drefAccess->getAsDoubleArray();
}

double * DrefStore::getDecimalArraySpecificIndices(const std::string& name, int index1, int index2)
{

	if (!drefExists(name)) {
		return nullptr;
	}

	shared_ptr<Dref> drefAccess = drefStore[name];
	return drefAccess->getDoubleArraySpecificIndex(index1,index2);
}

double DrefStore::getDecimalArrayIndex(const std::string& name, int index)
{

	if (!drefExists(name)) {
		return 0.0;
	}

	shared_ptr<Dref> drefAccess = drefStore[name];
	return drefAccess->getDoubleArrayIndex(index);
}



void DrefStore::setDecimalArray(const std::string& name,double values[], int offset)
{
	if (!drefExists(name)) {
		return;
	}

	shared_ptr<Dref> drefAccess = drefStore[name];

	drefAccess->setDoubleArray(values, offset);

}

void DrefStore::setDecimalArrayIndex(const std::string& name,double value, int index)
{

	if (!drefExists(name)) {
		return;
	}

	shared_ptr<Dref> drefAccess = drefStore[name];
	
	drefAccess->setDoubleArrayIndex(value, index);
}


void DrefStore::logNotFound(const string& name) {
	logger->setLogLevel(Logger::log_error);
	logger->logString(" Dataref "+name+" Not found / initialized. Please initiaize before performing any operations.");
}

void DrefStore::logIncorrectType() {
	logger->setLogLevel(Logger::log_error);
	logger->logString(" Incorrect operation on wrong dataref type. ");
}

bool DrefStore::drefExists(const std::string & name)
{
	if (drefStore.find(name) == drefStore.end()) {
		return false;
		logNotFound(name);
	}
	
	return true;
}

void DrefStore::logUnsupportedMethod() {
	logger->setLogLevel(Logger::log_error);
	logger->logString(" Unsupported method for array type! ");
}


shared_ptr<Dref> DrefStore::getDrefReference(const std::string& name)
{
	return drefExists(name) ? drefStore.at(name) : nullptr;
}



DrefStore::~DrefStore()
{

}

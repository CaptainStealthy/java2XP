#pragma once

#include <vector>
#include <string>
#include <utility/Logger.h>
#include<XPLMDataAccess.h>
#include <map>

class dref;
class Logger;

using namespace std;

class DrefStore
{
public:

	DrefStore();
	
	static enum drefTypes {
		integer,
		decimal,
		string_dref,
		int_array,
		decimal_array
	};


	static shared_ptr<DrefStore> getInstance();

	static shared_ptr<DrefStore> createNewStore();

	void initialize();
	static void stop();
	void update();
	void set(std::string name, std::string value);
	void add(std::string type, std::string simLocation, std::string name);

	void setInt(std::string name, int value);
	void setDecimal(std::string name, double value);
	void setString(std::string name, std::string value);
	

	void setDecimalArray(std::string name, double values[], int offset);

	void setDecimalArrayIndex(std::string name,double value, int index);
	void logUnsupportedMethod();

	void remove(std::string name);
	//std::string get(std::string name);
	
	int getInt(std::string name);
	double getDecimal(std::string name);
	std::string getString(std::string name);

	double* getDecimalArray(std::string name);
	double* getDecimalArraySpecificIndices(std::string name, int index1, int index2);
	double getDeimalArrayIndex(std::string name, int index);


	shared_ptr<dref> getDrefReference(std::string name);
	
	~DrefStore();

private:
	void logNotFound(std::string name);
	void logIncorrectType();
	std::map<std::string, std::shared_ptr<dref>> drefStore;
	std::unique_ptr<Logger> logger;
	
	static shared_ptr<DrefStore> storeInstance;

};


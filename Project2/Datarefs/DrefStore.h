#pragma once

#include <vector>
#include <string>
#include <utility/Logger.h>
#include<XPLMDataAccess.h>
#include <map>

class Dref;
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


	void initialize();
	void update();

	
	void add(const std::string& type, const std::string& simLocation,const std::string& name);

	void setInt(const std::string& name, int value);
	int getInt(const std::string& name);


	double getDecimal(const std::string& name);
	void setDecimal(const std::string& name, double value);


	void setString(const std::string& name, const std::string& value);
	std::string getString(const std::string& name);

	void setDecimalArray(const std::string& name, double values[], int offset);
	void setDecimalArrayIndex(const std::string& name,double value, int index);
	double getDecimalArrayIndex(const std::string& name, int index);
	double* getDecimalArray(const std::string& name);
	double* getDecimalArraySpecificIndices(const std::string& name, int index1, int index2);

	void remove(const std::string& name);
	
	
	shared_ptr<Dref> getDrefReference(const std::string& name);
	
	~DrefStore();

	void logUnsupportedMethod();

private:
	void logNotFound(const std::string& name);
	void logIncorrectType();
	std::map<std::string, std::shared_ptr<Dref>> drefStore;
	std::unique_ptr<Logger> logger;
	bool drefExists(const std::string& name);


};


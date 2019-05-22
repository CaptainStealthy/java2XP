#pragma once
#include<vector>
#include <map>
#include <string>
using namespace std;

class GlobalStore
{
public:

	static enum drefTypes {
		integer,
		decimal,
		string_dref,
		int_array,
		decimal_array
	};

	static GlobalStore* singleInstance;

	static inline GlobalStore* getInstance() {
		if (!singleInstance) {
			singleInstance = new GlobalStore();
		}
		return singleInstance;
	}
	
	void addProperty(std::string property, std::string value);
	void setProperty(std::string property, std::string value);

	static vector<string> split(const string& str, const string& delim)
	{
		vector<string> tokens;
		size_t prev = 0, pos = 0;
		do
		{
			pos = str.find(delim, prev);
			if (pos == string::npos) pos = str.length();
			string token = str.substr(prev, pos - prev);
			if (!token.empty()) tokens.push_back(token);
			prev = pos + delim.length();
		} while (pos < str.length() && prev < str.length());
		return tokens;
	}


	std::string getProperty(std::string property);
	
private:
	~GlobalStore();
	GlobalStore();
};


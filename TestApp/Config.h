/**
*  @file Config.h
*  @brief Used to load in key value pairs of settings
*
*  Loads in settings from a file in the format: key=pair
*
*
*  Values can be retrieved as an Int, float, double, bool, or string but the user must know what format to ask for
*	
*  @author Sam Murphy
*  @bug No known bugs.
*/

#pragma once
#include <map>
#include <string>

typedef std::map<std::string, std::string> ConfigInfo;
typedef std::map<std::string, std::string>::iterator ConfigInfoIterator;

/**
*  Loads and stores key-value pair settings from a config file.
*
*
*/
class Config
{
public:
	Config();
	~Config();

	/// All key-value pair settings
	ConfigInfo configValues;

	void ReadInFromFile(const std::string& filePath);

	std::string& GetValue(const std::string& key);

	int GetValueAsInt(const std::string& key);
	float GetValueAsFloat(const std::string& key);
	double GetValueAsDouble(const std::string& key);
	bool GetValueAsBool(const std::string& key);



private:
	/// Empty string, to return on failure.
	std::string empty;

};

/**
*  Retrieves the value with the key "key" from configValues, by iterating through the map
*
*  @param key The key value of the setting you're looking for in string format.
*  @return The value with the key "key" as a string, returns an empty string if no value is found.
*/
inline std::string& Config::GetValue(const std::string& key)
{
	// Found
	ConfigInfoIterator i = configValues.find(key);
	if (i != configValues.end())
	{
		return i->second;
	}
	// Not found
	empty = "";
	return empty;
}




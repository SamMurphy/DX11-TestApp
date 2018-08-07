/**
*  @file Config.cpp
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
#include "Config.h"
#include <iostream>
#include <sstream>
#include <fstream>


Config::Config()
{
}


Config::~Config()
{
}

/**
*  Reads in the config file from "filePath", and stores all the settings in configValues. Values must be in the format: key=value.
*  Lines beggining with '#' are ignored as comments.
*
*  @param filePath The path including file extension to the config file.
*/
void Config::ReadInFromFile(const std::string & filePath)
{
	std::ifstream fileStream;
	fileStream.open(filePath);	
	std::string line;

	if (fileStream.is_open())
	{
		while (std::getline(fileStream, line))
		{
			std::istringstream is_line(line);
			std::string key;
			if (std::getline(is_line, key, '='))
			{
				std::string value;
				if (key[0] == '#')
					continue;

				if (std::getline(is_line, value))
				{
					configValues[key] = value;
				}
			}
		}
	}
}

/**
*  Retrieves the value with the key "key" from configValues as an int.
*
*  @param key The key value of the setting you're looking for in string format.
*  @return The value with the key "key" as an int, returns 0 if no value is found.
*/
int Config::GetValueAsInt(const std::string & key)
{
	std::string value = GetValue(key);

	int i = 0;

	try
	{
		i = std::stoi(value);
	}
	catch(...)
	{
		printf("failed to parse %s value to int\n", key.c_str());
		return 0;
	}

	return i;
}

/**
*  Retrieves the value with the key "key" from configValues as an float.
*
*  @param key The key value of the setting you're looking for in string format.
*  @return The value with the key "key" as an float, returns 0.0f if no value is found.
*/
float Config::GetValueAsFloat(const std::string & key)
{
	std::string value = GetValue(key);

	float f = 0;

	try
	{
		f = std::stof(value);
	}
	catch (...)
	{
		printf("failed to parse %s value to float\n", key.c_str());
		return 0;
	}

	return f;
}

/**
*  Retrieves the value with the key "key" from configValues as an double.
*
*  @param key The key value of the setting you're looking for in string format.
*  @return The value with the key "key" as an double, returns 0.0 if no value is found.
*/
double Config::GetValueAsDouble(const std::string & key)
{
	std::string value = GetValue(key);

	double d = 0;

	try
	{
		d = std::stod(value);
	}
	catch (...)
	{
		printf("failed to parse %s value to double\n", key.c_str());
		return 0;
	}

	return d;
}

/**
*  Retrieves the value with the key "key" from configValues as a bool.
*
*  @param key The key value of the setting you're looking for in string format.
*  @return The value with the key "key" as an bool, returns false if no value is found.
*/
bool Config::GetValueAsBool(const std::string& key)
{
	std::string value = GetValue(key);

	if (value.compare("true") == 0 ||
		value.compare("True") == 0 ||
		value.compare("TRUE") == 0)
	{
		return true;
	}
	return false;
}

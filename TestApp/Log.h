/**
*  @file Log.h
*  @brief Single header logger, with severity levels.
*
*  A single header file logger, with severity levels, and file output.
*
*  @author Sam Murphy
*  @bug No known bugs.
*/



#ifndef __LOG_H__
#define __LOG_H__

#include <sstream>
#include <string>
#include <stdio.h>
#include "Globals.h"

/**
*  @brief The severity levels for the logger
*/
enum Severity
{
	ERR = 0,
	WARNING = 1,
    INFO = 2,
	DEBUG = 3,
	VERBOSE = 4
};

template <typename T>
class Log
{
public:
	Log();
	virtual ~Log();

	std::ostringstream& CreateLog(Severity level = INFO, const char* function = "", long line = -1, const char* file = "");

public:

	static Severity& ReportingLevel();
	static bool FileName();
	static std::string ToString(Severity level);
	static Severity FromString(const std::string& level);
	static void IncludeFileName(bool b);

protected:

	std::ostringstream os;

private:

	Log(const Log&);
	Log& operator =(const Log&);

};

template <typename T>
Log<T>::Log()
{
}

template <typename T>
std::ostringstream& Log<T>::CreateLog(Severity level, const char* function, long line, const char* file)
{
	// Create severity level string
	std::string levelString = ToString(level);
	levelString += std::string(levelString.length() < 7 ? 7 - levelString.length() : 0, ' ');

	os << levelString << " ";

	// Function and line number
	if (function != "" && line != -1)
		os << "[" << function << ":" << line << "] ";

	// File name
	if (file != "" && FileName())
		os << "(" << file << ") ";

	// Pass the stream off, to get the rest of the message
	return os;
}

template <typename T>
Log<T>::~Log()
{
	// Add an end line.
	os << std::endl;
	// Actually output the string
	T::Print(os.str());
}

template <typename T>
Severity& Log<T>::ReportingLevel()
{
	static Severity reportingLevel = VERBOSE;
	return reportingLevel;
}

template <typename T>
bool Log<T>::FileName()
{
	static bool fileName = false;
	return fileName;
}

template <typename T>
void Log<T>::IncludeFileName(bool b)
{
	fileName = b;
}

template <typename T>
std::string Log<T>::ToString(Severity level)
{
	static const char* const buffer[] = { "ERROR", "WARNING", "INFO", "DEBUG", "VERBOSE" };
	return buffer[level];
}

template <typename T>
Severity Log<T>::FromString(const std::string& level)
{
	if (level == "DEBUG")
		return DEBUG;
	if (level == "VERBOSE")
		return VERBOSE;
	if (level == "INFO")
		return INFO;
	if (level == "WARNING")
		return WARNING;
	if (level == "ERROR")
		return ERR;
	Log<T>().CreateLog(WARNING) << "Unknown logging level '" << level << "'. Using DEBUG level as default.";
	return DEBUG;
}

class Output
{
public:
	static FILE*& Stream();
	static void Print(const std::string& msg);
};

inline FILE*& Output::Stream()
{
	static FILE* pStream = stderr;
	return pStream;
}

inline void Output::Print(const std::string& msg)
{
	FILE* pStream = Stream();

	if (!pStream)
		return;

	fprintf(pStream, "%s", msg.c_str());
	fflush(pStream);

	if (pStream != stderr)
	{
		fprintf(stderr, "%s", msg.c_str());
		fflush(stderr);
	}

#if defined D_USE_IMGUI
	GlobalSettings::Settings().imGuiAppLog.AddLog(msg.c_str());
#endif
}

#ifndef FILELOG_DECLSPEC
#define FILELOG_DECLSPEC
#endif

class FILELOG_DECLSPEC Logger : public Log<Output> {};
//typedef Log<Output2FILE> FILELog;

#ifndef FILELOG_MAX_LEVEL
#define FILELOG_MAX_LEVEL VERBOSE
#endif

// Trims __FILE__ into just the file name
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define LOG(level) \
    if (level > FILELOG_MAX_LEVEL) ;\
    else if (level > Logger::ReportingLevel() || !Output::Stream()) ; \
    else Logger().CreateLog(level, __FUNCTION__, __LINE__, __FILENAME__)

// Log calls
#define LOG_VERBOSE \
	LOG(VERBOSE)

#define LOG_DEBUG \
	LOG(DEBUG)

#define LOG_INFO \
	LOG(INFO)

#define LOG_WARNING \
	LOG(WARNING)

#define LOG_ERROR \
	LOG(ERR)

#endif //__LOG_H__